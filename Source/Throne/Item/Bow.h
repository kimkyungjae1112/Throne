// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bow.generated.h"

UCLASS()
class THRONE_API ABow : public AActor
{
	GENERATED_BODY()
	
public:	
	ABow();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE class USkeletalMeshComponent* GetMesh() const { return Bow; }

private:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> Bow;

};
