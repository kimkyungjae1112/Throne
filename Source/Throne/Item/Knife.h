// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/KnifeInterface.h"
#include "Knife.generated.h"

UCLASS()
class THRONE_API AKnife : public AActor, public IKnifeInterface
{
	GENERATED_BODY()
	
public:	
	AKnife();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetKnifeDirection(const FVector& InDirection);


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess))
	TObjectPtr<class USkeletalMeshComponent> KnifeMesh;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	TObjectPtr<class UBoxComponent> BoxCollision;

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	TObjectPtr<class UProjectileMovementComponent> PMC;
};
