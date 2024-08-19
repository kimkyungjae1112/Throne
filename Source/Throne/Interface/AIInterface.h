// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAIInterface : public UInterface
{
	GENERATED_BODY()
};


DECLARE_DELEGATE(FAIMonsterAttackFinished)
DECLARE_DELEGATE(FOnAimingFinished)
/**
 * 
 */
class THRONE_API IAIInterface
{
	GENERATED_BODY()

public:
	virtual float GetPatrolRadius() = 0;
	virtual float GetDetectRadius() = 0;
	virtual float GetAttackInRange() = 0;
	virtual float GetTurnSpeed() = 0;

	virtual void SetAIAttackDelegate(FAIMonsterAttackFinished AIMonsterAttackFinished) = 0;
	virtual void AttackByAI(class UAnimMontage* InAnimMontage) = 0;

	virtual void SetAimingDelegate(FOnAimingFinished OnAimingFinished) = 0;
	virtual void AimingByArcher() = 0;
};
