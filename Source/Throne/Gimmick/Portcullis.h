// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portcullis.generated.h"

UCLASS()
class THRONE_API APortcullis : public AActor
{
	GENERATED_BODY()
	
public:	
	APortcullis();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:

	void Open();
	void Close();

private:
	UPROPERTY(VisibleAnywhere, Category = "Portcullis")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, Category = "Portcullis")
	TObjectPtr<class UStaticMeshComponent> Mesh;

	bool bOpenFlag;
	bool bCloseFlag;
	FVector Start;
	float MoveTime;
};
