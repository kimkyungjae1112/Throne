// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy.h"
#include "EnemyBoss.generated.h"

/**
 * 
 */
UCLASS()
class THRONE_API AEnemyBoss : public AEnemy
{
	GENERATED_BODY()
	
public:
	AEnemyBoss();

protected:
	virtual void BeginPlay() override;

	
};
