// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimationPlayer/CharacterAnimInstance.h"
#include "EnemyArcherAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class THRONE_API UEnemyArcherAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()
	
public:
	UEnemyArcherAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
	uint8 bIsAiming : 1;
};
