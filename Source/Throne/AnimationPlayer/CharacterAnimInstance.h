// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class THRONE_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UCharacterAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, Category = "Pointer")
	TObjectPtr<class ACharacter> Character;

	UPROPERTY(VisibleAnywhere, Category = "Pointer")
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
	float MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
	float JumpingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
	uint8 bIsShield : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
	uint8 bIsDefault : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
	uint8 bIsAimKnife : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
	uint8 bCanClimbingLadder : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
	uint8 bCanClimbUp : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
	uint8 bCanClimbDown : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
	FVector Angle;

};
