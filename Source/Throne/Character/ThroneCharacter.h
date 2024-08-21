// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interface/HUDWidgetInterface.h"
#include "GimmickInterface/ItemAcquisitionInterface.h"
#include "GimmickInterface/GateLeverDelegateInterface.h"
#include "GimmickInterface/DoorInterface.h"
#include "GimmickInterface/DragonGateInterface.h"
#include "GimmickInterface/LadderInterface.h"
#include "ThroneCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterMode : uint8
{
	Default = 0,
	HoldWeapon,
	Ladder
};

UCLASS()
class THRONE_API AThroneCharacter : public ACharacter, public IHUDWidgetInterface, public IItemAcquisitionInterface, public IGateLeverDelegateInterface, public IDoorInterface, public IDragonGateInterface, public ILadderInterface
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
	
	virtual void SetGateLever(class AGateLever* InGateLever) override;
	virtual void SetDoorPointer(class ADoor* InDoor) override;
	virtual void SetDragonGate(class ADragonGate* InDragonGate) override;
	virtual void SetLadder(class ALadder* InLadder) override;

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
	TObjectPtr<class UInputAction> MoveLadderAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DefaultAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DefendAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SheathAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AimKnifeAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> FireKnifeAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> KickAction;

	void Move(const FInputActionValue& Value);	//Triggered WASD
	void MoveLadder(const FInputActionValue& Value); //Triggerd WS
	void LookUp(const FInputActionValue& Value);	//Tirggered Mouse 2D Axis
	void DefaultAttack();	//Started Mouse Left Click
	void BeginDefend();	//Triggerd Mouse Right Click
	void EndDefend();	//Completed Mouse Right Click
	void Kick();

	void AcquisitionItem();	//Started E
	void GateLeverInteract(); //Started E
	void DoorInteract(); //Started E
	void DragonGateInteract(); //Started E
	void LadderInteract(); //Started E
	void Sheath();	//Started Q
	void AttachWeaponSheath();	//Sheath
	void AttachWeaponHand();	//Sheath

	void BeginAimKnife(const FInputActionValue& Value);	//Trigger Mouse Right Click
	void EndAimKnife();	//Completed Mouse Right Click
	void FireKnife();	//Stated Mouse Left Click

/* Item */
//Weapon
private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<class USkeletalMeshComponent> Sword;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<class USkeletalMeshComponent> Shield;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<class USkeletalMeshComponent> Knife;
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TSubclassOf<class AKnife> KnifeClass;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<class UItemData> ItemData;

	ECharacterMode CurrentCharacterMode;
	ECharacterMode PrevMode;
	bool bHasWeapon = false;
	bool bIsAiming = false;
	bool bCooldownKnife = false;

//Gimmick
	UPROPERTY(VisibleAnywhere, Category = "Gimmick")
	TObjectPtr<class AGateLever> GateLever;

	UPROPERTY(VisibleAnywhere, Category = "Gimmick")
	TObjectPtr<class ADoor> Door;

	UPROPERTY(VisibleAnywhere, Category = "Gimmick")
	TObjectPtr<class ADragonGate> DragonGate;

	UPROPERTY(VisibleAnywhere, Category = "Gimmick")
	TObjectPtr<class ALadder> Ladder;

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterStatComponent> Stat;


/* Animation */
private:
	void Death();

/* Data Asset */
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data Asset", meta = (AllowPrivateAccess = "true"))
	TArray<class UCharacterAimKnifeData*> AimKnifeDatas;



/* Utility */
private:
	class AThronePlayerController* GetPlayerController();
	void SetAimKnifeData(const uint8 Index);
	uint8 CurrentAimKnife = 1;
};
