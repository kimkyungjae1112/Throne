// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy.h"
#include "EnemyKnight.generated.h"

/**
 * 
 */
UCLASS()
class THRONE_API AEnemyKnight : public AEnemy
{
	GENERATED_BODY()
	
public:
	AEnemyKnight();

protected:
	virtual void BeginPlay() override;

/* Overriding */
public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	TArray<class USkeletalMesh*> MeshArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UEnemyStatComponent> Stat;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> DeadMontage;

private:
	void SetDead();
};
