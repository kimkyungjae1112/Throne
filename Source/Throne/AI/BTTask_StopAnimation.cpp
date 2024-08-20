// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_StopAnimation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/EnemyArcher.h"
#include "Animation/AnimInstance.h"

UBTTask_StopAnimation::UBTTask_StopAnimation()
{
	NodeName = TEXT("StopAnimation");
}

EBTNodeResult::Type UBTTask_StopAnimation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* PossessPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (PossessPawn == nullptr)
	{
		EBTNodeResult::Failed;
	}

	AEnemyArcher* Enemy = Cast<AEnemyArcher>(PossessPawn);
	if (Enemy == nullptr)
	{
		EBTNodeResult::Failed;
	}

	UAnimInstance* AnimInstance = Enemy->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(0.5f);
	}
	
	Enemy->DetachmentArrow();

	return EBTNodeResult::Succeeded;
}
