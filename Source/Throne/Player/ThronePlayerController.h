// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ThronePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class THRONE_API AThronePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AThronePlayerController();

protected:
	virtual void BeginPlay() override;

	
public:
	void DisplayItemInteract();
	void HideItemInteract();
	bool IsDisplay();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UHUDWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UHUDWidget> HUDWidgetPtr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> ItemInteractClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUserWidget> ItemInteractPtr;
};
