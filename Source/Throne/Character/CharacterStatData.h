// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterStatData.generated.h"

/**
 * 
 */
UCLASS()
class THRONE_API UCharacterStatData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UCharacterStatData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float AttackRadian;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Enegy;

};
