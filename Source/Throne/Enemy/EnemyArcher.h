// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy.h"
#include "Interface/ArcherInterface.h"
#include "EnemyArcher.generated.h"

/**
 * 
 */
UCLASS()
class THRONE_API AEnemyArcher : public AEnemy, public IArcherInterface
{
	GENERATED_BODY()

public:
	AEnemyArcher();
	
protected:
	virtual void BeginPlay() override;

public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

/* AI Interface */
public:
	virtual void SetAimingDelegate(FOnAimingFinished OnAimingFinished) override;
	virtual void AimingByArcher() override;

/* Interface */
	virtual void ArrowFire() override;

	FOnAimingFinished AimingFinished;

public:
	void DetachmentArrow();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Bow")
	TSubclassOf<class ABow> BowClass;

	UPROPERTY(VisibleAnywhere, Category = "Bow")
	TObjectPtr<class ABow> Bow;

	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
	TSubclassOf<class AArrow> ArrowClass;

	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
	TObjectPtr<class USkeletalMesh> ArrowMesh;

	UPROPERTY(VisibleAnywhere, Category = "Arrow")
	TObjectPtr<class AArrow> Arrow;

	UPROPERTY(VisibleAnywhere, Category = "Arrow")
	TObjectPtr<class USkeletalMeshComponent> ArrowPos;
	
	UPROPERTY(EditAnywhere, Category = "Stat")
	TObjectPtr<class UEnemyStatComponent> Stat;

private:
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> AimMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> FireMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> BowStringPullMontage;

/* Animation Function */
private:
	
	void BeginAiming();
	void EndAiming(class UAnimMontage* Target, bool IsProperlyEnded);
	void BeginStringPull();
	void BeginStringLay();

	void SetDead();


/* Utility */
private:
	void SpawnArrow();
};
