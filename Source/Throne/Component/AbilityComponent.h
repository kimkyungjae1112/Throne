// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/AttackHitCheckInterface.h"
#include "AbilityComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnDefaultAttackUseEnergy, float /* Use Energy */)
DECLARE_DELEGATE(FOnInSheath)
DECLARE_DELEGATE(FOnOutSheath)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THRONE_API UAbilityComponent : public UActorComponent, public IAttackHitCheckInterface
{
	GENERATED_BODY()

public:	
	UAbilityComponent();

protected:
	virtual void BeginPlay() override;

public:
	FOnDefaultAttackUseEnergy OnDefaultAttackUseEnergy;
	FOnInSheath OnInSheath;
	FOnOutSheath OnOutSheath;

public:
	/* Default Attack */
	void BeginComboAttack();
	void BeginDefaultAttack();
	void EndDefaultAttack(class UAnimMontage* Target, bool IsProperlyEnded);
	void SetComboCheckTimer();
	void ComboCheck();

	/* Default Attack Hit Check */
	virtual void DefaultAttackHitCheck() override;
	bool CheckInRadialRange(AActor* Player, AActor* Target, float Radius, float RadialAngle);
	void AttackHitDebug(UWorld* World, const FVector& Start, const FVector& ForwardVector, const float AttackRange, const FColor& Color);

	/* Defend */
	void BeginShieldUp();
	void EndShieldUp();

	/* Dead */
	void BeginDead();

	/* Sheath */
	void BeginSheath();
	void EndSheath(class UAnimMontage* Target, bool IsProperlyEnded);
	void BeginOutSheath();
	void EndOutSheath(class UAnimMontage* Target, bool IsProperlyEnded);

	/* Aim Knife */
	void BeginAimKnife();
	void EndAimKnife();

	/* Jump Attack */
	void BeginJumpAttack();
	void EndJumpAttack(class UAnimMontage* Target, bool IsProperlyEnded);
	void SetPlayerController(class AThronePlayerController* InPlayerController);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AThronePlayerController> PlayerController;

private:
	/* Default Attack */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DefaultAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Asset", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterComboActionData> ComboActionData;

	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

	/* Dead */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	/* Sheath Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> SheathInMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> SheathOutMontage;
	
	/* Jump Attack */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> JumpAttackMontage;

private:
	/* Stat */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Asset", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterStatData> StatData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Asset", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterAimKnifeData> AimData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Asset", meta = (AllowPrivateAccess = "true"))
	float DefaultAttackUseEnergy;

};
