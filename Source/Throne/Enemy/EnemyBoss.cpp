// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBoss.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/EnemyStatComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "AI/AAIController.h"
#include "Engine/OverlapResult.h"
#include "Engine/DamageEvents.h"
#include "UI/ThroneWidgetComponent.h"
#include "UI/BossHpBarWidget.h"

AEnemyBoss::AEnemyBoss()
{
	//Capsule
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	//Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/DF_DRAGON_KNIGHT/MESHES/UE5/SK_Dragon_knight_UE5.SK_Dragon_knight_UE5'"));
	if (MeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	/* Item */
	Sword = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Sword"));
	Sword->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SwordMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/DF_DRAGON_KNIGHT/MESHES/SWORD/SK_Dragon_knight_sword.SK_Dragon_knight_sword'"));
	if (SwordMeshRef.Object)
	{
		Sword->SetSkeletalMesh(SwordMeshRef.Object);
	}

	/* Components */
	Stat = CreateDefaultSubobject<UEnemyStatComponent>(TEXT("Stat Component"));
	ThroneWidgetComponent = CreateDefaultSubobject<UThroneWidgetComponent>(TEXT("Widget Component"));
	ThroneWidgetComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FClassFinder<UBossHpBarWidget> WidgetClassRef(TEXT("/Game/Throne/UI/WBP_BossHpBar.WBP_BossHpBar_C"));
	if (WidgetClassRef.Class)
	{
		ThroneWidgetComponent->SetWidgetClass(WidgetClassRef.Class);
	}

	/* UI */
	static ConstructorHelpers::FClassFinder<UBossHpBarWidget> BossHpBarWidgetClassRef(TEXT("/Game/Throne/UI/WBP_BossHpBar.WBP_BossHpBar_C"));
	if (BossHpBarWidgetClassRef.Class)
	{
		BossHpBarWidgetClass = BossHpBarWidgetClassRef.Class;
	}

	/* Character Movement */
	GetCharacterMovement()->MaxWalkSpeed = Stat->GetMoveSpeed();
}

void AEnemyBoss::BeginPlay()
{
	Super::BeginPlay();

	Stat->OnHpZero.AddUObject(this, &AEnemyBoss::SetDead);

	SetUI();
}

/* AI Interface */
void AEnemyBoss::AttackByAI(UAnimMontage* InAnimMontage)
{
	Super::AttackByAI(DefaultAttackMontage);
}

/* overriding */
float AEnemyBoss::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Stat->ApplyDamage(Damage);

	return Damage;
}

void AEnemyBoss::DefaultAttackHitCheck()
{
	float ActualDamage = Stat->GetAttackDamage();
	float AttackRange = Stat->GetAttackRange();

	FVector Start = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FCollisionQueryParams Param(NAME_None, false, this);
	TArray<FOverlapResult> OverlapResults;
	FColor Color = FColor::Red;

	bool bHit = GetWorld()->OverlapMultiByChannel(OverlapResults, Start, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(AttackRange), Param);
	if(bHit)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AActor* Target = OverlapResult.GetActor();
			if (CheckInRadialRange(this, Target, AttackRange, Stat->GetAttackRadian()))
			{
				FDamageEvent DamageEvent;
				Target->TakeDamage(ActualDamage, DamageEvent, GetController(), this);
				
				Color = FColor::Green;
			}
		}
	}

	AttackHitDebug(GetWorld(), Start, ForwardVector, AttackRange, Color);
}

bool AEnemyBoss::CheckInRadialRange(AActor* Player, AActor* Target, float Radius, float RadialAngle)
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

void AEnemyBoss::AttackHitDebug(UWorld* World, const FVector& Start, const FVector& ForwardVector, const float AttackRange, const FColor& Color)
{
	float AngleRadians = FMath::DegreesToRadians(Stat->GetAttackRadian() / 2.0f);

	// 부채꼴의 두 끝점 계산
	FVector LeftVector = ForwardVector.RotateAngleAxis(-Stat->GetAttackRadian() / 2.0f, FVector::UpVector);
	FVector RightVector = ForwardVector.RotateAngleAxis(Stat->GetAttackRadian() / 2.0f, FVector::UpVector);

	FVector LeftEndpoint = Start + LeftVector * AttackRange;
	FVector RightEndpoint = Start + RightVector * AttackRange;

	// 부채꼴의 중심선
	DrawDebugLine(GetWorld(), Start, Start + ForwardVector * AttackRange, Color, false, 3.0f);

	// 부채꼴의 두 끝선
	DrawDebugLine(GetWorld(), Start, LeftEndpoint, Color, false, 3.0f);
	DrawDebugLine(GetWorld(), Start, RightEndpoint, Color, false, 3.0f);
}

void AEnemyBoss::JumpAttackDoneHitCheck()
{
}

void AEnemyBoss::SetDead()
{
	UAnimInstance* AnimInstacne = GetMesh()->GetAnimInstance();
	AAAIController* AIController = Cast<AAAIController>(GetController());

	if (AnimInstacne && AIController)
	{
		AIController->StopAI();

		AnimInstacne->Montage_Play(DeadMontage);
		SetActorEnableCollision(false);
	}
}

void AEnemyBoss::SetUI()
{
	BossHpBarWidgetPtr = CreateWidget<UBossHpBarWidget>(GetWorld(), BossHpBarWidgetClass);
	if (BossHpBarWidgetPtr)
	{
		BossHpBarWidgetPtr->SetMaxHp(Stat->GetMaxHp());
		BossHpBarWidgetPtr->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(BossHpBarWidgetPtr, &UBossHpBarWidget::UpdateHpBar);
		BossHpBarWidgetPtr->AddToViewport();
	}
}
