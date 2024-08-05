// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDWidget.h"
#include "UI/HpBarWidget.h"
#include "UI/EnergyBarWidget.h"
#include "Interface/HUDWidgetInterface.h"

UHUDWidget::UHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	HpBarWidget = Cast<UHpBarWidget>(GetWidgetFromName(TEXT("WBP_HpBar")));
	ensure(HpBarWidget);

	EnergyBarWidget = Cast<UEnergyBarWidget>(GetWidgetFromName(TEXT("WBP_EnergyBar")));
	ensure(EnergyBarWidget);

	IHUDWidgetInterface* HUDInterface = Cast<IHUDWidgetInterface>(GetOwningPlayerPawn());
	if (HUDInterface)
	{
		HUDInterface->SetHUD(this);
	}
}

void UHUDWidget::UpdateHpBar(float NewHp)
{
	HpBarWidget->UpdateBar(NewHp);
}

void UHUDWidget::UpdateEnergyBar(float NewEnergy)
{
	EnergyBarWidget->UpdateBar(NewEnergy);
}

void UHUDWidget::SetMaxHp(float InMaxHp)
{
	HpBarWidget->SetMaxHp(InMaxHp);
}

void UHUDWidget::SetMaxEnergy(float InMaxEnergy)
{
	EnergyBarWidget->SetMaxEnergy(InMaxEnergy);
}

