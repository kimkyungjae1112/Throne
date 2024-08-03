// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/OverlapResult.h"
#include "Interface/AIInterface.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PossessPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (PossessPawn == nullptr)
	{
		return;
	}

	UWorld* World = PossessPawn->GetWorld();
	if (World == nullptr)
	{
		return;
	}

	IAIInterface* AIPawn = Cast<IAIInterface>(PossessPawn);
	if (AIPawn == nullptr)
	{
		return;
	}

	FVector Origin = PossessPawn->GetActorLocation();
	const float Detect = AIPawn->GetDetectRadius();
	FCollisionQueryParams Params(NAME_None, true, PossessPawn);


	TArray<FOverlapResult> OverlapResults;
	bool bHit = World->OverlapMultiByChannel(OverlapResults, Origin, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(Detect), Params);
	if (bHit)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* TargetPawn = Cast<APawn>(OverlapResult.GetActor());
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), TargetPawn);
			return;
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
}
