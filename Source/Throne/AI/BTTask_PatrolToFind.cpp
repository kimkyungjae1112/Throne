// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_PatrolToFind.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Interface/AIInterface.h"

UBTTask_PatrolToFind::UBTTask_PatrolToFind()
{
	NodeName = TEXT("Patrol");
}

EBTNodeResult::Type UBTTask_PatrolToFind::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* PossessPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (PossessPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(PossessPawn->GetWorld());
	if (NavSystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IAIInterface* AIPawn = Cast<IAIInterface>(PossessPawn);
	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("HomePos"));
	const float PatrolRadius = AIPawn->GetPatrolRadius();
	FNavLocation NextPatrolPos;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, PatrolRadius, NextPatrolPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolPos"), NextPatrolPos);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
