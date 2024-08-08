// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterControlData.generated.h"

/**
 * 
 */
UCLASS()
class THRONE_API UCharacterControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UCharacterControlData();

	UPROPERTY(EditAnywhere, Category = "IMC")
	TObjectPtr<class UInputMappingContext> InputMappingContext;
	
};
