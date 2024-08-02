// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HpBarWidget.h"
#include "Components/ProgressBar.h"

UHpBarWidget::UHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBarPtr = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpBar")));
	ensure(HpBarPtr);

}

void UHpBarWidget::UpdateBar(float NewHp)
{
	HpBarPtr->SetPercent(NewHp / MaxHp);
}
