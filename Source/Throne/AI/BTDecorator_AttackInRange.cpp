// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackInRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/AIInterface.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	NodeName = TEXT("AttackInRange");
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool SuperResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* PossessPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (PossessPawn == nullptr)
	{
		return false;
	}

	IAIInterface* AIPawn = Cast<IAIInterface>(PossessPawn);
	if (AIPawn == nullptr)
	{
		return false;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	
	const float AttackRange = AIPawn->GetAttackInRange();
	const float Distance = FVector::Distance(Target->GetActorLocation(), PossessPawn->GetActorLocation());

	if (AttackRange >= Distance)
	{
		return true;
	}

	return false;
}
