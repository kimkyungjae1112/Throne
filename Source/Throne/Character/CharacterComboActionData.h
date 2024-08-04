// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class THRONE_API UCharacterComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UCharacterComboActionData();

	UPROPERTY(EditAnywhere, Category = "Name")
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = "Name")
	uint8 MaxComboCount;

	UPROPERTY(EditAnywhere, Category = "Rate")
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = "ComboData")
	TArray<float> EffectiveFrameCount;

};
