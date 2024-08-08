// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ThroneWidgetComponent.h"
#include "UI/BossHpBarWidget.h"
void UThroneWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UBossHpBarWidget* ThroneUserWidget = Cast<UBossHpBarWidget>(GetWidget());
	if (ThroneUserWidget)
	{
		ThroneUserWidget->SetOwningActor(GetOwner()); 
	}
}
