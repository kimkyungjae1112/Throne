// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UENUM(BlueprintType)
enum class EDoorType : uint8
{
	Open = 0,
	Close
};

UCLASS()
class THRONE_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void OnDoorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnDoorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void OnTriggerDoor();

private:
	UPROPERTY(VisibleAnywhere, Category = "Door")
	TObjectPtr<class USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, Category = "Door")
	TObjectPtr<class UBoxComponent> Box;

	UPROPERTY(VisibleAnywhere, Category = "Door")
	TObjectPtr<class UStaticMeshComponent> Mesh;
	
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UUserWidget> WidgetPtr;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> WidgetClass;

	EDoorType CurrentDoorType;
	FVector PlayerLocation;
	FVector DoorLocation;
	FVector Direction;

	FRotator InitialRotation;
	FRotator FrontTargetRotation;
	FRotator BackTargetRotation;

	bool bDoorFlag;
	
	UPROPERTY(EditAnywhere)
	float DoorTurnRate;
	
};
