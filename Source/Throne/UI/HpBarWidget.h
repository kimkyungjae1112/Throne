// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class THRONE_API UHpBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UHpBarWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	FORCEINLINE void SetMaxHp(const float InMaxHp) { MaxHp = InMaxHp; }
public:
	void UpdateBar(float NewHp);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta =(AllowPrivateAccess = "true"))
	TObjectPtr<class UProgressBar> HpBarPtr;

	float MaxHp;
	
};
