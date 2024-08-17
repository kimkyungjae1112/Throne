// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimationEnemy/BossAttackHitCheckAnimNotify.h"
#include "Interface/BossAttackHitCheckInterface.h"

UBossAttackHitCheckAnimNotify::UBossAttackHitCheckAnimNotify()
{
}

void UBossAttackHitCheckAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IBossAttackHitCheckInterface* Interface = Cast<IBossAttackHitCheckInterface>(MeshComp->GetOwner());
	if (Interface)
	{
		Interface->DefaultAttackHitCheck();
	}
}
