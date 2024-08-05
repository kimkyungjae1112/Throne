// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

/**
 * 
 */
UCLASS()
class THRONE_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UItemData();

	UPROPERTY(EditAnywhere, Category = "Sword")
	TObjectPtr<class USkeletalMesh> SwordMesh;

	UPROPERTY(EditAnywhere, Category = "Shield")
	TObjectPtr<class USkeletalMesh> ShieldMesh;

	
};
