// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimationEnemy/EnemyAttackHitCheckNotify.h"
#include "Interface/EnemyAttackHitCheckInterface.h"

UEnemyAttackHitCheckNotify::UEnemyAttackHitCheckNotify()
{
}

void UEnemyAttackHitCheckNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IEnemyAttackHitCheckInterface* AttackInterface = Cast<IEnemyAttackHitCheckInterface>(MeshComp->GetOwner());
	if (AttackInterface)
	{
		AttackInterface->DefaultAttackHitCheck();
	}
}
