// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

UCLASS()
class THRONE_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	AArrow();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE float GetDelayedTime() const { return DelayedTime; }

	void SetDirection(const FVector& InDirection);
	void ActiveMovement();

private:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> Arrow;

	UPROPERTY(EditAnywhere, Category = "Collision")
	TObjectPtr<class UBoxComponent> Box;

	UPROPERTY(VisibleAnywhere, Category = "PMC")
	TObjectPtr<class UProjectileMovementComponent> PMC;

private:
	UPROPERTY(EditAnywhere)
	float DelayedTime;

	FVector Direction;
};
