// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyKnight.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Component/EnemyStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/AAIController.h"

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
