// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyAttackHitCheckInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyAttackHitCheckInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THRONE_API IEnemyAttackHitCheckInterface
{
	GENERATED_BODY()

public:
	virtual void DefaultAttackHitCheck() = 0;
};
