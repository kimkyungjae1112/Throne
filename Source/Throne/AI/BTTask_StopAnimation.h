// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StopAnimation.generated.h"

/**
 * 
 */
UCLASS()
class THRONE_API UBTTask_StopAnimation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_StopAnimation();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
