// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ladder.generated.h"

DECLARE_DELEGATE(FOnTopEndClimb)
DECLARE_DELEGATE(FOnBottomEndClimb)

UCLASS()
class THRONE_API ALadder : public AActor
{
	GENERATED_BODY()
	
public:	
	ALadder();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	FOnTopEndClimb OnTopEndClimb;
	FOnBottomEndClimb OnBottomEndClimb;

public:
	UFUNCTION()
	void OnLadderTopBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnLadderTopEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnLadderBottomBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnLadderBottomEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void OnLadderClimb();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spline", meta = (AllowPrivateAccess))
	TObjectPtr<class USplineComponent> SplineComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess))
	TObjectPtr<UStaticMesh> LadderFrame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess))
	TObjectPtr<UStaticMesh> LadderStep;

	UPROPERTY()
	TArray<UStaticMeshComponent*> LadderStepComponents;

	UPROPERTY()
	TArray<UStaticMeshComponent*> LadderFrameComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger", meta = (AllowPrivateAccess))
	TObjectPtr<class UBoxComponent> TopBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger", meta = (AllowPrivateAccess))
	TObjectPtr<class UBoxComponent> BottomBox;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widget", meta = (AllowPrivateAccess))
	TObjectPtr<class UUserWidget> WidgetPtr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widget", meta = (AllowPrivateAccess))
	TSubclassOf<class UUserWidget> WidgetClass;


private:
	bool bIsPlayer;

	void ClearLadderMesh();

};
