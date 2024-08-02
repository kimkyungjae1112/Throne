// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AAIController.generated.h"

/**
 * 
 */
UCLASS()
class THRONE_API AAAIController : public AAIController
{
	GENERATED_BODY()

public:
	AAAIController();
	
protected:
	virtual void OnPossess(APawn* InPawn) override;

public:
	void RunAI();
	void StopAI();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Asset", meta = (AllowPrivateAccess))
	TObjectPtr<class UBlackboardData> BBData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Asset", meta = (AllowPrivateAccess))
	TObjectPtr<class UBehaviorTree> BTAsset;
};
