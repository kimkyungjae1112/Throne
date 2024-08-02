// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ThronePlayerController.h"
#include "UI/HUDWidget.h"

AThronePlayerController::AThronePlayerController()
{
	static ConstructorHelpers::FClassFinder<UHUDWidget> HUDWidgetClassRef(TEXT("/Game/Throne/UI/WBP_HUD.WBP_HUD_C"));
	if (HUDWidgetClassRef.Class)
	{
		HUDWidgetClass = HUDWidgetClassRef.Class;
	}
}

void AThronePlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	HUDWidgetPtr = CreateWidget<UHUDWidget>(this, HUDWidgetClass);
	if (HUDWidgetPtr)
	{
		HUDWidgetPtr->AddToViewport();
	}
}
