// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ArcherArrowSpawnNotify.generated.h"

/**
 * 
 */
UCLASS()
class THRONE_API UArcherArrowSpawnNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UArcherArrowSpawnNotify();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
