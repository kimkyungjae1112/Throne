// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interface/HUDWidgetInterface.h"
#include "Interface/ItemAcquisitionInterface.h"
#include "ThroneCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterMode : uint8
{
	Default = 0,
	HoldWeapon
};

UCLASS()
class THRONE_API AThroneCharacter : public ACharacter, public IHUDWidgetInterface, public IItemAcquisitionInterface
{
	GENERATED_BODY()

public:
	AThroneCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
/* overriding */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


/* Interface */
	virtual void SetHUD(class UHUDWidget* InHUDWidget) override;
	virtual void BeginOverlapTakeItem(class UItemData* InItemData) override;
	virtual void EndOverlapTakeItem() override;

/* Camera */
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

/* Input */
private:
	//Input Mapping Context
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterMode InCharacterMode);
	
	UPROPERTY(EditAnywhere, Category = "CharacterControl", meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterMode, class UCharacterControlData*> CharacterControlManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DefaultAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DefendAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RollAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SheathAction;

	void Move(const FInputActionValue& Value);	//Triggered WASD
	void LookUp(const FInputActionValue& Value);	//Tirggered Mouse 2D Axis
	void DefaultAttack();	//Started Mouse Left Click
	void BeginDefend();	//Triggerd Mouse Right Click
	void EndDefend();	//Completed Mouse Right Click
	void Roll(); //Started Shift
	void AcquisitionItem();	//Started E
	void Sheath();	//Started Q
	void AttachWeaponSheath();	//Sheath
	void AttachWeaponHand();	//Sheath

	

/* Item */
private:
	UPROPERTY(VisibleAnywhere, Category = "Item")
	TObjectPtr<class USkeletalMeshComponent> Sword;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	TObjectPtr<class USkeletalMeshComponent> Shield;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	TObjectPtr<class UItemData> ItemData;

	ECharacterMode CurrentCharacterMode;
	bool bHasWeapon = false;

/* UI */
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UHUDWidget> HUDWidget;


/* Components */
//Attack
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAbilityComponent> Ability;

//Stat
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterStatComponent> Stat;

	void DefaultAttackUseEnergy(float UseEnergy);


/* Animation */
private:

	void Death();




/* Utility */
private:
	class AThronePlayerController* GetPlayerController();

};
