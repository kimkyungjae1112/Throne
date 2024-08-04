// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZero)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChanged, float /* CurrentHp */)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THRONE_API UEnemyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyStatComponent();

	FORCEINLINE const float GetAttackDamage() const { return AttackDamage; }
	FORCEINLINE const float GetAttackRange() const { return AttackRange; }
	FORCEINLINE const float GetAttackSpeed() const { return AttackSpeed; }
	FORCEINLINE const float GetAttackRadian() const { return AttackRadian; }
	FORCEINLINE const float GetMoveSpeed() const { return MoveSpeed; }
	FORCEINLINE const float GetMaxHp() const { return MaxHp; }
	FORCEINLINE const float GetCurrentHp() const { return CurrentHp; }

	FOnHpZero OnHpZero;
	FOnHpChanged OnHpChanged;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	float ApplyDamage(float InDamage);
	void SetHp(float NewHp);
		
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat Data", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UEnemyStatData> StatData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float AttackRadian;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float CurrentHp;

};
