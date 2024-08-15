// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BossHpBarWidget.h"
#include "Components/ProgressBar.h"

UBossHpBarWidget::UBossHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UBossHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBarPtr = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpBar")));
	if (!HpBarPtr)
	{
		UE_LOG(LogTemp, Error, TEXT("HpBarPtr is NULL. Ensure that the widget named 'HpBar' exists in the UMG widget tree."));
		HpBarPtr->SetPercent(1.0f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HpBarPtr successfully initialized."));
	}
}

void UBossHpBarWidget::UpdateHpBar(float NewHp)
{
	if (HpBarPtr)
	{
		HpBarPtr->SetPercent(NewHp / MaxHp);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HpBarPtr is NULL. Cannot update HP bar."));
	}
}
