// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/JumpAttackDoneNotify.h"
#include "Component/AbilityComponent.h"
#include "Interface/AttackHitCheckInterface.h"

UJumpAttackDoneNotify::UJumpAttackDoneNotify()
{
}

void UJumpAttackDoneNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IAttackHitCheckInterface* Interface = Cast<IAttackHitCheckInterface>(MeshComp->GetOwner()->GetComponentByClass<UAbilityComponent>());
	if (Interface)
	{
		Interface->JumpAttackDoneHitCheck();
	}
}
