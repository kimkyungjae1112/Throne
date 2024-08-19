// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Aiming.generated.h"

/**
 * 
 */
UCLASS()
class THRONE_API UBTTask_Aiming : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Aiming();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
