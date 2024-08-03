// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interface/HUDWidgetInterface.h"
#include "ThroneCharacter.generated.h"

UCLASS()
class THRONE_API AThroneCharacter : public ACharacter, public IHUDWidgetInterface
{
	GENERATED_BODY()

public:
	AThroneCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

/* Interface */
	virtual void SetHUD(class UHUDWidget* InHUDWidget) override;

/* Camera */
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

/* Input */
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> IMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DefaultAttackAction;


	void Move(const FInputActionValue& Value);	//Triggered WASD
	void LookUp(const FInputActionValue& Value);	//Tirggered Mouse 2D Axis
	void DefaultAttack();	//Started Mouse Left Click


/* Item */
private:
	UPROPERTY(VisibleAnywhere, Category = "Item")
	TObjectPtr<class USkeletalMeshComponent> Sword;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	TObjectPtr<class USkeletalMeshComponent> Shield;

/* UI */
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UHUDWidget> HUDWidget;

/* Components */

//Attack
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAbilityComponent> Ability;

//Stat
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterStatComponent> Stat;

};
