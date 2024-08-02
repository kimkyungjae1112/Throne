// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnergyBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class THRONE_API UEnergyBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UEnergyBarWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	
	FORCEINLINE void SetMaxEnergy(const float InMaxEnergy) { MaxEnergy = InMaxEnergy; }
public:
	void UpdateBar(float NewEnergy);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProgressBar> EnergyBar;

	float MaxEnergy;
};
