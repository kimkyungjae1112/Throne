// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy.h"
#include "Interface/AttackHitCheckInterface.h"
#include "Interface/BossWidgetInterface.h"
#include "EnemyBoss.generated.h"

/**
 * 
 */
UCLASS()
class THRONE_API AEnemyBoss : public AEnemy, public IAttackHitCheckInterface, public IBossWidgetInterface
{
	GENERATED_BODY()
	
public:
	AEnemyBoss();

protected:
	virtual void BeginPlay() override;

/* AI Interface */
public:
	virtual void AttackByAI(class UAnimMontage* InAnimMontage) override;
	
/* Overriding*/
public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

/* Interface*/
public:
	virtual void DefaultAttackHitCheck() override;
	bool CheckInRadialRange(AActor* Player, AActor* Target, float Radius, float RadialAngle);
	void AttackHitDebug(UWorld* World, const FVector& Start, const FVector& ForwardVector, const float AttackRange, const FColor& Color);

	virtual void SetWidget(class UBossHpBarWidget* InBossHpBarWidget) override;


/* Item */
private:
	UPROPERTY(VisibleAnywhere, Category = "Item")
	TObjectPtr<class USkeletalMeshComponent> Sword;
	

/* Animation */
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DefaultAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	void SetDead();

/* Components */
private:
	//Stat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UEnemyStatComponent> Stat;

	//UI
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UThroneWidgetComponent> ThroneWidgetComponent;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UBossHpBarWidget> BossHpBarWidget;
};
