// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimationEnemy/ArcherArrowSpawnNotify.h"

UArcherArrowSpawnNotify::UArcherArrowSpawnNotify()
{
}

void UArcherArrowSpawnNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);


}
