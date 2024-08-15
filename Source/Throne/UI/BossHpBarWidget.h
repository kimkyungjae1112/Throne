// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class THRONE_API UBossHpBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UBossHpBarWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	
public:
	FORCEINLINE void SetMaxHp(float InMaxHp) { MaxHp = InMaxHp; }
	FORCEINLINE void SetOwningActor(AActor* NewOwner) { OwningActor = NewOwner; }

	void UpdateHpBar(float NewHp);

private:
	UPROPERTY(VisibleAnywhere, Category = "Actor")
	TObjectPtr<AActor> OwningActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProgressBar> HpBarPtr;

	float MaxHp;
};
