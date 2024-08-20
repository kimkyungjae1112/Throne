// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimationEnemy/ArcherArrowSpawnNotify.h"
#include "Interface/ArcherInterface.h"

UArcherArrowSpawnNotify::UArcherArrowSpawnNotify()
{
}

void UArcherArrowSpawnNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IArcherInterface* Interface = Cast<IArcherInterface>(MeshComp->GetOwner());
	if (Interface)
	{
		Interface->ArrowFire();
	}

}
