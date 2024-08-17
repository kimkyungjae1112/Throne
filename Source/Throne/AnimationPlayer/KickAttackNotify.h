// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "KickAttackNotify.generated.h"

/**
 * 
 */
UCLASS()
class THRONE_API UKickAttackNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UKickAttackNotify();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
