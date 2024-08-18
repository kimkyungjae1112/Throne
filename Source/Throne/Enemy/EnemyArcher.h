// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy.h"
#include "EnemyArcher.generated.h"

/**
 * 
 */
UCLASS()
class THRONE_API AEnemyArcher : public AEnemy
{
	GENERATED_BODY()

public:
	AEnemyArcher();
	


private:
	UPROPERTY(VisibleAnywhere, Category = "Bow")
	TObjectPtr<class USkeletalMeshComponent> Bow;

	UPROPERTY(EditAnywhere, Category = "Stat")
	TObjectPtr<class UEnemyStatComponent> Stat;
};
