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

/* AI Interface */
public:
	virtual void AttackByAI(class UAnimMontage* InAnimMontage) override;
	

/* Item */
private:
	UPROPERTY(VisibleAnywhere, Category = "Item")
	TObjectPtr<class USkeletalMeshComponent> Sword;
	
/* Animation */
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DefaultAttackMontage;
};
