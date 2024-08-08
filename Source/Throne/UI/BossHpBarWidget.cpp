// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BossHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/BossWidgetInterface.h"

UBossHpBarWidget::UBossHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UBossHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBarPtr = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpBar")));
	ensure(HpBarPtr);

	IBossWidgetInterface* HpBarInterface = Cast<IBossWidgetInterface>(OwningActor);
	if (HpBarInterface)
	{
		HpBarInterface->SetWidget(this);
	}
}

void UBossHpBarWidget::UpdateHpBar(float NewHp)
{
	UE_LOG(LogTemp, Warning, TEXT("Update HpBar "));
	UE_LOG(LogTemp, Warning, TEXT("MaxHp : %f"), MaxHp);
	HpBarPtr->SetPercent(NewHp / MaxHp);	
}
