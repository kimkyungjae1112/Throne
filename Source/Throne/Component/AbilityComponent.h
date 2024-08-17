// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/AttackHitCheckInterface.h"
#include "Gimmick/DragonGate.h"
#include "AbilityComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnDefaultAttackUseEnergy, float /* Use Energy */)
DECLARE_DELEGATE_OneParam(FOnJumpAttackUseEnergy, float /* Use Energy */)
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
	FOnJumpAttackUseEnergy OnJumpAttackUseEnergy;
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

	/* Jump Attack Hit Check*/
	virtual void JumpAttackDoneHitCheck() override;

	/* Gimmick */ /* 기믹에 있는 End 함수들 없애도 되지만 기능 확장시 필요할 것 같아 남겨둠 */
	void BeginLeverOpen();
	void EndLeverOpen(class UAnimMontage* Target, bool IsProperlyEnded);
	void BeginLeverClose();
	void EndLeverClose(class UAnimMontage* Target, bool IsProperlyEnded);

	void BeginDragonGateOpen(EGateType GateType);
	void EndDragonGateOpen(class UAnimMontage* Target, bool IsProperlyEnded);

	void BeginLadderBottomStart();
	void EndLadderTopStart();

	/* Utility */
	void SetPlayerController(class AThronePlayerController* InPlayerController);

	UPROPERTY(EditAnywhere)
	float DefaultAttackUseEnergy;

	UPROPERTY(EditAnywhere)
	float JumpAttackUseEnergy;

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

	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<class UParticleSystem> JumpAttackParticle;

	/* Gimmick */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> OpenLeverMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> CloseLeverMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> OpenSmallDragonGateMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> OpenNormalDragonGateMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> OpenBigDragonGateMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> LadderBottomStartMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> LadderTopStartMontage;


private:
	/* Stat */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Asset", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterStatData> StatData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Asset", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterAimKnifeData> AimData;
};
