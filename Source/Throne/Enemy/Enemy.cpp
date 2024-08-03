// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AEnemy::GetPatrolRadius()
{
	return 800.0f;
}

float AEnemy::GetDetectRadius()
{
	return 800.0f;
}

float AEnemy::GetAttackInRange()
{
	return 200.0f;
}

float AEnemy::GetTurnSpeed()
{
	return 1.0f;
}

void AEnemy::SetAIAttackDelegate(FAIMonsterAttackFinished AIMonsterAttackFinished)
{
	AttackFinishedDeletage = AIMonsterAttackFinished;
}

void AEnemy::AttackByAI(class UAnimMontage* InAnimMontage)
{
	BeginDefaultAttack(InAnimMontage);
}

void AEnemy::BeginDefaultAttack(UAnimMontage* AnimMontage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AnimMontage)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		AnimInstance->Montage_Play(AnimMontage);

		FOnMontageEnded MontageEnded;
		MontageEnded.BindUObject(this, &AEnemy::EndDefaultAttack);
		AnimInstance->Montage_SetEndDelegate(MontageEnded, AnimMontage);
	}
}

void AEnemy::EndDefaultAttack(UAnimMontage* Target, bool IsProperlyEnded)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	AttackFinishedDeletage.ExecuteIfBound();
}

