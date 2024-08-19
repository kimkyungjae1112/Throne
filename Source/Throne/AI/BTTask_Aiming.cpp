// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Aiming.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Interface/AIInterface.h"

UBTTask_Aiming::UBTTask_Aiming()
{
	NodeName = TEXT("Aiming");
}

EBTNodeResult::Type UBTTask_Aiming::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	APawn* PossessPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (PossessPawn == nullptr)
	{
		EBTNodeResult::Failed;
	}

	IAIInterface* AIPawn = Cast<IAIInterface>(PossessPawn);
	if (AIPawn == nullptr)
	{
		EBTNodeResult::Failed;
	}

	FOnAimingFinished OnAimingFinished;
	OnAimingFinished.BindLambda(
		[&]()
		{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	AIPawn->SetAimingDelegate(OnAimingFinished);
	AIPawn->AimingByArcher();

	return EBTNodeResult::InProgress;
}
