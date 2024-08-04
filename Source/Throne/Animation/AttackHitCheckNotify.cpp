// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AttackHitCheckNotify.h"
#include "Interface/AttackHitCheckInterface.h"
#include "Component/AbilityComponent.h"

UAttackHitCheckNotify::UAttackHitCheckNotify()
{
}

void UAttackHitCheckNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IAttackHitCheckInterface* Interface = Cast<IAttackHitCheckInterface>(MeshComp->GetOwner()->GetComponentByClass<UAbilityComponent>());
	if (Interface)
	{
		Interface->DefaultAttackHitCheck();
	}
}
