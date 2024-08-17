// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LadderInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULadderInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THRONE_API ILadderInterface
{
	GENERATED_BODY()

public:
	virtual void SetLadder(class ALadder* InLadder) = 0;
};
