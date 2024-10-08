// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/AbilityComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Character/CharacterComboActionData.h"
#include "Character/CharacterStatData.h"
#include "Character/CharacterAimKnifeData.h"
#include "Engine/OverlapResult.h"
#include "Engine/DamageEvents.h"
#include "AnimationPlayer/CharacterAnimInstance.h"
#include "Player/ThronePlayerController.h"
#include "Kismet/GameplayStatics.h"

UAbilityComponent::UAbilityComponent()
{
	DefaultAttackUseEnergy = 10.0f;
	JumpAttackUseEnergy = 30.0f;
	KnifeFireUseEnergy = 20.0f;
}


// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UAbilityComponent::BeginComboAttack()
{
	if (CurrentCombo == 0)
	{
		BeginDefaultAttack();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}

void UAbilityComponent::BeginDefaultAttack()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();
	if (Owner && AnimInstance)
	{
		CurrentCombo = 1;
		Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		AnimInstance->Montage_Play(DefaultAttackMontage, StatData->AttackSpeed);
		OnDefaultAttackUseEnergy.ExecuteIfBound(DefaultAttackUseEnergy);

		FOnMontageEnded MontageEnded;
		MontageEnded.BindUObject(this, &UAbilityComponent::EndDefaultAttack);
		AnimInstance->Montage_SetEndDelegate(MontageEnded, DefaultAttackMontage);
	
		ComboTimerHandle.Invalidate();
		SetComboCheckTimer();
	}
}

void UAbilityComponent::EndDefaultAttack(UAnimMontage* Target, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;

	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();
	if (Owner && AnimInstance)
	{
		Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void UAbilityComponent::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / StatData->AttackSpeed;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UAbilityComponent::ComboCheck, ComboEffectiveTime, false);
	}
}

void UAbilityComponent::ComboCheck()
{
	ComboTimerHandle.Invalidate();
	if (HasNextComboCommand)
	{
		ACharacter* Owner = Cast<ACharacter>(GetOwner());
		UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();

		if (Owner && AnimInstance)
		{
			CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
			FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
			AnimInstance->Montage_JumpToSection(NextSection, DefaultAttackMontage);
			OnDefaultAttackUseEnergy.ExecuteIfBound(DefaultAttackUseEnergy);

			SetComboCheckTimer();
			HasNextComboCommand = false;
		}
	}
}

void UAbilityComponent::DefaultAttackHitCheck()
{
	AActor* Owner = GetOwner();
	ensure(Owner);

	float AttackDamage = StatData->AttackDamage;
	float AttackRange = StatData->AttackRange;

	FVector Start = GetOwner()->GetActorLocation();
	FVector ForwardVector = GetOwner()->GetActorForwardVector();
	FColor Color = FColor::Red;

	FCollisionQueryParams Params(NAME_None, false, Owner);

	TArray<FOverlapResult> OverlapResults;
	bool bHit = GetWorld()->OverlapMultiByChannel(OverlapResults, Start, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(AttackRange), Params);
	if (bHit)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			FDamageEvent DamageEvent;
			if (CheckInRadialRange(Owner, OverlapResult.GetActor(), AttackRange, StatData->AttackRadian))
			{
				APawn* OwnerPawn = Cast<APawn>(Owner);
				OverlapResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, OwnerPawn->GetController(), Owner);
				Color = FColor::Green;
			}
		}
	}
	DrawDebugSphere(GetWorld(), Start, AttackRange, 16, Color, false, 3.0f);
	AttackHitDebug(GetWorld(), Start, ForwardVector, AttackRange, Color);
}

bool UAbilityComponent::CheckInRadialRange(AActor* Player, AActor* Target, float Radius, float RadialAngle)
{
	if (!Player || !Target) return false;

	FVector PlayerLocation = Player->GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();
	FVector ForwardVector = Player->GetActorForwardVector();
	FVector DirectionToTarget = (TargetLocation - PlayerLocation).GetSafeNormal();

	// 타겟과의 거리 계산
	float DistanceToTarget = FVector::Dist(PlayerLocation, TargetLocation);

	// 타겟이 반경 내에 있는지 확인
	if (DistanceToTarget >= Radius) return false;

	// 타겟이 부채꼴 각도 내에 있는지 확인
	float DotProduct = FVector::DotProduct(ForwardVector, DirectionToTarget);
	float AngleToTarget = FMath::Acos(DotProduct);

	// 라디안에서 각도로 변환
	float AngleToTargetDegrees = FMath::RadiansToDegrees(AngleToTarget);

	return AngleToTargetDegrees <= (RadialAngle / 2.0f);
}

void UAbilityComponent::AttackHitDebug(UWorld* World, const FVector& Start, const FVector& ForwardVector, const float AttackRange, const FColor& Color)
{
	float AngleRadians = FMath::DegreesToRadians(StatData->AttackRadian / 2.0f);

	// 부채꼴의 두 끝점 계산
	FVector LeftVector = ForwardVector.RotateAngleAxis(-StatData->AttackRadian / 2.0f, FVector::UpVector);
	FVector RightVector = ForwardVector.RotateAngleAxis(StatData->AttackRadian / 2.0f, FVector::UpVector);

	FVector LeftEndpoint = Start + LeftVector * AttackRange;
	FVector RightEndpoint = Start + RightVector * AttackRange;

	// 부채꼴의 중심선
	DrawDebugLine(GetWorld(), Start, Start + ForwardVector * AttackRange, Color, false, 3.0f);

	// 부채꼴의 두 끝선
	DrawDebugLine(GetWorld(), Start, LeftEndpoint, Color, false, 3.0f);
	DrawDebugLine(GetWorld(), Start, RightEndpoint, Color, false, 3.0f);
}

void UAbilityComponent::BeginShieldUp()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->bIsShield = true;
		Owner->GetCharacterMovement()->MaxWalkSpeed = StatData->WalkMoveSpeed;
	}
}

void UAbilityComponent::EndShieldUp()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->bIsShield = false;
		Owner->GetCharacterMovement()->MaxWalkSpeed = StatData->RunMoveSpeed;
	}
}

void UAbilityComponent::BeginDead()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();
	if (Owner && AnimInstance)
	{
		Owner->SetActorEnableCollision(false);
		AnimInstance->Montage_Play(DeadMontage);
	}
}

void UAbilityComponent::BeginSheath()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();

	if (Owner && AnimInstance)
	{
		Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		AnimInstance->Montage_Play(SheathInMontage);

		FOnMontageEnded MontageEnded;
		MontageEnded.BindUObject(this, &UAbilityComponent::EndSheath);
		AnimInstance->Montage_SetEndDelegate(MontageEnded, SheathInMontage);
	}
}

void UAbilityComponent::EndSheath(class UAnimMontage* Target, bool IsProperlyEnded)
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(Owner->GetMesh()->GetAnimInstance());

	if (Owner && AnimInstance)
	{
		OnInSheath.ExecuteIfBound();
		Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	
		AnimInstance->bIsDefault = true;
	}
}

void UAbilityComponent::BeginOutSheath()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();

	if (Owner && AnimInstance)
	{
		Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		OnOutSheath.ExecuteIfBound();

		AnimInstance->Montage_Play(SheathOutMontage);

		FOnMontageEnded MontageEnded;
		MontageEnded.BindUObject(this, &UAbilityComponent::EndOutSheath);
		AnimInstance->Montage_SetEndDelegate(MontageEnded, SheathOutMontage);
	}
}

void UAbilityComponent::EndOutSheath(class UAnimMontage* Target, bool IsProperlyEnded)
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(Owner->GetMesh()->GetAnimInstance());

	if (Owner && AnimInstance)
	{
		Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		AnimInstance->bIsDefault = false;
	}
}

void UAbilityComponent::BeginAimKnife()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(Owner->GetMesh()->GetAnimInstance());

	if (Owner && AnimInstance)
	{
		AnimInstance->bIsAimKnife = true;
	}
}

void UAbilityComponent::EndAimKnife()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(Owner->GetMesh()->GetAnimInstance());

	if (Owner && AnimInstance)
	{
		AnimInstance->bIsAimKnife = false;
	}
}

void UAbilityComponent::BeginJumpAttack()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UAnimInstance* AnimInstance = Cast<UAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	if (Owner && AnimInstance)
	{		
		Owner->DisableInput(PlayerController);		
		AnimInstance->Montage_Play(JumpAttackMontage);
		OnJumpAttackUseEnergy.ExecuteIfBound(JumpAttackUseEnergy);

		FOnMontageEnded MontageEnded;
		MontageEnded.BindUObject(this, &UAbilityComponent::EndJumpAttack);
		AnimInstance->Montage_SetEndDelegate(MontageEnded, JumpAttackMontage);
	}
}

void UAbilityComponent::EndJumpAttack(class UAnimMontage* Target, bool IsProperlyEnded)
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	ensure(Owner);

	Owner->EnableInput(PlayerController);
}

void UAbilityComponent::JumpAttackDoneHitCheck()
{
	float Damage = StatData->AttackDamage * 2;
	float Range = StatData->AttackRange;

	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	FVector Start = Owner->GetActorLocation() + FVector(30.0f, 0.0f, 0.0f);
	FCollisionQueryParams Params(NAME_None, false, GetOwner());
	TArray<FOverlapResult> OverlapResults;

	UGameplayStatics::SpawnEmitterAtLocation(Owner->GetWorld(), JumpAttackParticle, Start);

	bool bHit = GetWorld()->OverlapMultiByChannel(OverlapResults, Start, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Range), Params);
	if (bHit)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			FDamageEvent DamageEvent;
			OverlapResult.GetActor()->TakeDamage(Damage, DamageEvent, Owner->GetController(), Owner);
		}
	}
}

void UAbilityComponent::BeginKickAttack()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();
	if (Owner && AnimInstance)
	{
		AnimInstance->Montage_Play(KickMontage);
		Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		OnDefaultAttackUseEnergy.ExecuteIfBound(DefaultAttackUseEnergy);

		FOnMontageEnded MontageEnded;
		MontageEnded.BindUObject(this, &UAbilityComponent::EndKickAttack);
		AnimInstance->Montage_SetEndDelegate(MontageEnded, KickMontage);
	}
}

void UAbilityComponent::EndKickAttack(class UAnimMontage* Target, bool IsProperlyEnded)
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (Owner)
	{
		Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void UAbilityComponent::KickAttackHitCheck()
{
	float Damage = 300.0f;
	float Range = 200.0f;
	
	UE_LOG(LogTemp, Display, TEXT("실행됨"));
	FVector Start = GetOwner()->GetActorLocation();
	FVector End = Start + FVector(Range, 0.0f, 0.0f);
	FCollisionQueryParams Params(NAME_None, false, GetOwner());
	
	TArray<FHitResult> HitResults;
	bool bHit = GetWorld()->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, ECC_GameTraceChannel2 ,FCollisionShape::MakeBox(FVector3f(Range, Range, Range)), Params);
	if (bHit)
	{
		for (auto const& HitResult : HitResults)
		{
			ACharacter* Actor = Cast<ACharacter>(HitResult.GetActor());
			FDamageEvent DamagEvent;
			Actor->TakeDamage(Damage, DamagEvent, PlayerController, GetOwner());

			FVector Direction(-500.0f, 0.0f, 0.0f);
			Actor->LaunchCharacter(Direction, true, false);
		}
	}

}

/******* Gimmick *******/
void UAbilityComponent::BeginLeverOpen()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UAnimInstance* AnimInstance = Cast<UAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	if (Owner && AnimInstance)
	{
		Owner->SetActorEnableCollision(false);
		AnimInstance->Montage_Play(OpenLeverMontage);

		FOnMontageEnded MontageEnded;
		MontageEnded.BindUObject(this, &UAbilityComponent::EndLeverOpen);
		AnimInstance->Montage_SetEndDelegate(MontageEnded, OpenLeverMontage);
	}
}

void UAbilityComponent::EndLeverOpen(class UAnimMontage* Target, bool IsProperlyEnded)
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UAnimInstance* AnimInstance = Cast<UAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	if (Owner && AnimInstance)
	{
		Owner->SetActorEnableCollision(true);
	}
}

void UAbilityComponent::BeginLeverClose()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UAnimInstance* AnimInstance = Cast<UAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	if (Owner && AnimInstance)
	{
		Owner->SetActorEnableCollision(false);
		AnimInstance->Montage_Play(CloseLeverMontage);

		FOnMontageEnded MontageEnded;
		MontageEnded.BindUObject(this, &UAbilityComponent::EndLeverClose);
		AnimInstance->Montage_SetEndDelegate(MontageEnded, CloseLeverMontage);
	}
}

void UAbilityComponent::EndLeverClose(class UAnimMontage* Target, bool IsProperlyEnded)
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UAnimInstance* AnimInstance = Cast<UAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	if (Owner && AnimInstance)
	{
		Owner->SetActorEnableCollision(true);
	}
}

void UAbilityComponent::BeginDragonGateOpen(EGateType GateType)
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UAnimInstance* AnimInstance = Cast<UAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	
	if (Owner && AnimInstance)
	{
		switch (GateType)
		{
		case EGateType::Small:
			AnimInstance->Montage_Play(OpenSmallDragonGateMontage);
			break;
		case EGateType::Normal:
			AnimInstance->Montage_Play(OpenNormalDragonGateMontage);
			break;
		case EGateType::Big:
			AnimInstance->Montage_Play(OpenBigDragonGateMontage);
			break;
		default:
			return;
		}
	}

}


void UAbilityComponent::BeginLadderBottomStart()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	if (Owner && AnimInstance)
	{
		AnimInstance->Montage_Play(LadderBottomStartMontage);
	
		AnimInstance->bCanClimbingLadder = true;
	}
}

void UAbilityComponent::BeginLadderBottomEnd()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	if (Owner && AnimInstance)
	{
		AnimInstance->Montage_Play(LadderBottomEndMontage);

		AnimInstance->bCanClimbingLadder = false;

		FOnMontageEnded MontageEnd;
		MontageEnd.BindUObject(this, &UAbilityComponent::EndLadderBottomEnd);
		AnimInstance->Montage_SetEndDelegate(MontageEnd, LadderBottomEndMontage);
	}
}

void UAbilityComponent::EndLadderBottomEnd(class UAnimMontage* Target, bool IsProperlyEnded)
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (Owner)
	{
		Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}


void UAbilityComponent::BeginLadderTopStart()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	if (Owner && AnimInstance)
	{
		UE_LOG(LogTemp, Display, TEXT("LadderTopStart"));
		AnimInstance->Montage_Play(LadderTopStartMontage);

		AnimInstance->bCanClimbingLadder = true;
	}
}

void UAbilityComponent::BeginLadderTopEnd()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	if (Owner && AnimInstance)
	{
		AnimInstance->Montage_Play(LadderTopEndMontage);

		AnimInstance->bCanClimbingLadder = false;

		FOnMontageEnded MontageEnd;
		MontageEnd.BindUObject(this, &UAbilityComponent::EndLadderTopEnd);
		AnimInstance->Montage_SetEndDelegate(MontageEnd, LadderTopEndMontage);
	}
}

void UAbilityComponent::EndLadderTopEnd(class UAnimMontage* Target, bool IsProperlyEnded)
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (Owner)
	{
		UE_LOG(LogTemp, Display, TEXT("TopEnd End 몽타주"));

		Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}


void UAbilityComponent::SetPlayerController(AThronePlayerController* InPlayerController)
{
	PlayerController = InPlayerController;
}

