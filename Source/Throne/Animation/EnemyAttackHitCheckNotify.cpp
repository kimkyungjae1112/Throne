// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/EnemyAttackHitCheckNotify.h"
#include "Interface/AttackHitCheckInterface.h"

UEnemyAttackHitCheckNotify::UEnemyAttackHitCheckNotify()
{
}

void UEnemyAttackHitCheckNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IAttackHitCheckInterface* AttackInterface = Cast<IAttackHitCheckInterface>(MeshComp->GetOwner());
	if (AttackInterface)
	{
		AttackInterface->DefaultAttackHitCheck();
	}
}
