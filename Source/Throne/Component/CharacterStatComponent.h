// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZero)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChanged, float /* CurrentHp */)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnergyChanged, float /* CurrentEnergy */)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THRONE_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatComponent();
	
	FORCEINLINE const float GetAttackDamage() const { return AttackDamage; }
	FORCEINLINE const float GetAttackRange() const { return AttackRange; }
	FORCEINLINE const float GetAttackSpeed() const { return AttackSpeed; }
	FORCEINLINE const float GetAttackRadian() const { return AttackRadian; }
	FORCEINLINE const float GetWalkMoveSpeed() const { return WalkMoveSpeed; }
	FORCEINLINE const float GetRunMoveSpeed() const { return RunMoveSpeed; }
	FORCEINLINE const float GetMaxHp() const { return MaxHp; }
	FORCEINLINE const float GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE const float GetMaxEnergy() const { return MaxEnergy; }
	FORCEINLINE const float GetCurrentEnergy() const { return CurrentEnergy; }

	FOnHpZero OnHpZero;
	FOnHpChanged OnHpChanged;
	FOnEnergyChanged OnEnergyChanged;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	float ApplyDamage(float InDamage);
	void SetHp(float NewHp);
	void SetEnergy(float UsedEnergy);

private:
	UPROPERTY(VisibleAnywhere, Category = "Stat Data")
	TObjectPtr<class UCharacterStatData> StatData;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float AttackDamage;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float AttackRange;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float AttackSpeed;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float AttackRadian;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float WalkMoveSpeed;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float RunMoveSpeed;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float MaxHp;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float CurrentHp;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float MaxEnergy;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float CurrentEnergy;

private:
	float EnergyRegenTime;
	float HealthRegenTime;
};
