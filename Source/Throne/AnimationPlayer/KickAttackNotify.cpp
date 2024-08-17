// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimationPlayer/KickAttackNotify.h"
#include "Interface/AttackHitCheckInterface.h"
#include "Component/AbilityComponent.h"

UKickAttackNotify::UKickAttackNotify()
{
}

void UKickAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IAttackHitCheckInterface* Interface = Cast<IAttackHitCheckInterface>(MeshComp->GetOwner()->GetComponentByClass<UAbilityComponent>());
	if (Interface)
	{
		Interface->KickAttackHitCheck();
	}
}
