// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterAimKnifeData.generated.h"

/**
 * 
 */
UCLASS()
class THRONE_API UCharacterAimKnifeData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UCharacterAimKnifeData();

	UPROPERTY(EditAnywhere, Category = "Camera")
	float TargetArmLength;

	UPROPERTY(EditAnywhere, Category = "Spring Arm")
	uint8 UsePawnControlRotation : 1;

	UPROPERTY(EditAnywhere, Category = "Character Movement")
	uint8 OrientRotationToMovement : 1;
	
	UPROPERTY(EditAnywhere, Category = "Character Movement")
	uint8 UseControllerDesiredRotation : 1;
	
	UPROPERTY(EditAnywhere, Category = "Pawn")
	uint8 UseControllerRotationPitch : 1;

	UPROPERTY(EditAnywhere, Category = "Pawn")
	uint8 UseControllerRotationRoll : 1;

	UPROPERTY(EditAnywhere, Category = "Pawn")
	uint8 UseControllerRotationYaw : 1;

	UPROPERTY(EditAnywhere, Category = "Ability")
	uint8 bCanThrow : 1;

	UPROPERTY(EditAnywhere, Category = "XYAxis")
	float MouseMoveSpeedRate;

};
