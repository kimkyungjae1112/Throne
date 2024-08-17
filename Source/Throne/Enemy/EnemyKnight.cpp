// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyKnight.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Component/EnemyStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/AAIController.h"
#include "Engine/DamageEvents.h"

AEnemyKnight::AEnemyKnight()
{
	/* Mesh Array */
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Knight_01_Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/SKnight_modular/Skeleton_Knight_01/mesh/SK_SKnigh_01_full.SK_SKnigh_01_full'"));
	if (Knight_01_Ref.Object)
	{
		MeshArray.Add(Knight_01_Ref.Object);
	}

	/* Mesh */
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	/*static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponRef(TEXT("/Script/Engine.SkeletalMesh'/Game/SKnight_modular/Skeleton_Knight_01/mesh/weapon/SK_weapon_01.SK_weapon_01'"));
	if (WeaponRef.Object)
	{
		Weapon->SetSkeletalMesh(WeaponRef.Object);
	}*/

	Shield = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shield"));
	Shield->SetupAttachment(GetMesh(), TEXT("hand_lSocket"));
	/*static ConstructorHelpers::FObjectFinder<USkeletalMesh> ShieldRef(TEXT("/Script/Engine.SkeletalMesh'/Game/SKnight_modular/Skeleton_Knight_01/mesh/weapon/SK_shield_01.SK_shield_01'"));
	if (ShieldRef.Object)
	{
		Shield->SetSkeletalMesh(ShieldRef.Object);
	}*/

	/* Capsule */
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	/* Component */
	Stat = CreateDefaultSubobject<UEnemyStatComponent>(TEXT("Stat"));

	/* Character Movement */
	GetCharacterMovement()->MaxWalkSpeed = Stat->GetMoveSpeed();

}

void AEnemyKnight::BeginPlay()
{
	Super::BeginPlay();

	Stat->OnHpZero.AddUObject(this, &AEnemyKnight::SetDead);
}

void AEnemyKnight::AttackByAI(UAnimMontage* InAnimMontage)
{
	Super::AttackByAI(AttackMontage);
}

void AEnemyKnight::DefaultAttackHitCheck()
{
	float Damage = Stat->GetAttackDamage();
	float Range = Stat->GetAttackRange();
	float Radius = 50.0f;

	FColor Color = FColor::Red;
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * Range;
	FCollisionQueryParams Params(NAME_None, false, this);
	
	FHitResult HitResult;
	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(Radius), Params);
	if (bHit)
	{
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(Damage, DamageEvent, GetController(), this);
		Color = FColor::Green;
	}
	DrawDebugCapsule(GetWorld(), Start + GetActorForwardVector() * Range * 0.5, Range * 0.5 + Radius, Radius, FRotationMatrix::MakeFromZ(GetActorForwardVector() * Range).ToQuat(), Color, false, 3.0f);
}

float AEnemyKnight::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	BeginHitReaction();
	Stat->ApplyDamage(Damage);
	return Damage;
}

void AEnemyKnight::SetDead()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AAAIController* AIController = Cast<AAAIController>(GetController());

	if (AnimInstance && AIController)
	{
		AIController->StopAI();

		AnimInstance->Montage_Play(DeadMontage);
		SetActorEnableCollision(false);
	}
}

void AEnemyKnight::BeginHitReaction()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(HitReactionMontage);

		FOnMontageEnded MontageEnd;
		MontageEnd.BindUObject(this, &AEnemyKnight::EndHitReaction);
		AnimInstance->Montage_SetEndDelegate(MontageEnd, HitReactionMontage);
	}
}

void AEnemyKnight::EndHitReaction(UAnimMontage* Target, bool IsProperlyEnded)
{
}
