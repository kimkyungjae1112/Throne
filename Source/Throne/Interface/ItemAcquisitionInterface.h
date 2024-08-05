// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemAcquisitionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemAcquisitionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THRONE_API IItemAcquisitionInterface
{
	GENERATED_BODY()

public:
	virtual void TakeItem(class UItemData* ItemData) = 0;
};
