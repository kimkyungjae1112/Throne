// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/AIInterface.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* PossessPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (PossessPawn == nullptr)
	{
		EBTNodeResult::Failed;
	}

	IAIInterface* AIPawn = Cast<IAIInterface>(PossessPawn);
	if (AIPawn)
	{
		EBTNodeResult::Failed;
	}

	FAIMonsterAttackFinished AIMonsterAttackFinished;
	AIMonsterAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	AIPawn->SetAIAttackDelegate(AIMonsterAttackFinished);
	AIPawn->AttackByAI(nullptr);

	return EBTNodeResult::InProgress;
}
