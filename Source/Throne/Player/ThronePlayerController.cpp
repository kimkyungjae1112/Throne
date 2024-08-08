// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ThronePlayerController.h"
#include "UI/HUDWidget.h"
#include "UI/BossHpBarWidget.h"

AThronePlayerController::AThronePlayerController()
{
	static ConstructorHelpers::FClassFinder<UHUDWidget> HUDWidgetClassRef(TEXT("/Game/Throne/UI/WBP_HUD.WBP_HUD_C"));
	if (HUDWidgetClassRef.Class)
	{
		HUDWidgetClass = HUDWidgetClassRef.Class;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> ItemInteractClassRef(TEXT("/Game/Throne/UI/WBP_Interact.WBP_Interact_C"));
	if (ItemInteractClassRef.Class)
	{
		ItemInteractClass = ItemInteractClassRef.Class;
	}
	static ConstructorHelpers::FClassFinder<UBossHpBarWidget> BossHpBarWidgetClassRef(TEXT("/Game/Throne/UI/WBP_BossHpBar.WBP_BossHpBar_C"));
	if (BossHpBarWidgetClassRef.Class)
	{
		BossHpBarWidgetClass = BossHpBarWidgetClassRef.Class;
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
	
	ItemInteractPtr = CreateWidget<UUserWidget>(this, ItemInteractClass);
	BossHpBarWidgetPtr = CreateWidget<UBossHpBarWidget>(this, BossHpBarWidgetClass);
	if (BossHpBarWidgetPtr)
	{
		BossHpBarWidgetPtr->AddToViewport();
	}
}

void AThronePlayerController::DisplayItemInteract()
{
	if (ItemInteractPtr)
	{
		ItemInteractPtr->AddToViewport();
	}
}

void AThronePlayerController::HideItemInteract()
{
	if (ItemInteractPtr && ItemInteractPtr->IsInViewport())
	{
		ItemInteractPtr->RemoveFromViewport();
	}
}

bool AThronePlayerController::IsDisplay()
{
	if (ItemInteractPtr && ItemInteractPtr->IsInViewport())
	{
		return true;
	}
	return false;
}
