// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnergyBarWidget.h"
#include "Components/ProgressBar.h"

UEnergyBarWidget::UEnergyBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxEnergy = -1.0f;
}

void UEnergyBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	EnergyBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("EnergyBar")));
	ensure(EnergyBar);
}

void UEnergyBarWidget::UpdateBar(float NewEnergy)
{
	EnergyBar->SetPercent(NewEnergy / MaxEnergy);
}
