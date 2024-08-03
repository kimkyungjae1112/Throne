// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/AIInterface.h"
#include "Enemy.generated.h"

UCLASS()
class THRONE_API AEnemy : public ACharacter, public IAIInterface
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/* AI Interface */
	virtual float GetPatrolRadius();
	virtual float GetDetectRadius();
	virtual float GetAttackInRange();
	virtual float GetTurnSpeed();

	virtual void SetAIAttackDelegate(FAIMonsterAttackFinished AIMonsterAttackFinished);
	virtual void AttackByAI(class UAnimMontage* InAnimMontage);

	/* Delegate */
	FAIMonsterAttackFinished AttackFinishedDeletage;

private:
	void BeginDefaultAttack(class UAnimMontage* AnimMontage);
	void EndDefaultAttack(class UAnimMontage* Target, bool IsProperlyEnded);

};
