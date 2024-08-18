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
	
protected:
	virtual void BeginPlay() override;

public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Bow")
	TSubclassOf<class ABow> BowClass;

	UPROPERTY(VisibleAnywhere, Category = "Bow")
	TObjectPtr<class ABow> Bow;

	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
	TSubclassOf<class AArrow> ArrowClass;

	UPROPERTY(VisibleAnywhere, Category = "Arrow")
	TObjectPtr<class AArrow> Arrow;
	
	UPROPERTY(EditAnywhere, Category = "Stat")
	TObjectPtr<class UEnemyStatComponent> Stat;

/* Animation Function */
private:
	void SetDead();

};
