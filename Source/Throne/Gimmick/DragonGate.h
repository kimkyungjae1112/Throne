// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DragonGate.generated.h"

UENUM(BlueprintType)
enum class EGateType : uint8
{
	Small = 0,
	Normal,
	Big
};

UCLASS()
class THRONE_API ADragonGate : public AActor
{
	GENERATED_BODY()
	
public:	
	ADragonGate();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE EGateType GetGateType() const { return CurrentType; }
	FORCEINLINE FVector GetInteractLocation() const { return InteractPos->GetComponentLocation(); }
	FORCEINLINE FRotator GetInteractRotation() const { return InteractPos->GetComponentRotation(); }

	UFUNCTION()
	void OnDragonGateBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnDragonGateEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void OnDragonGateTrigger();

private:
	UPROPERTY(VisibleAnywhere, Category = "Gate")
	TObjectPtr<class USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, Category = "Gate")
	TObjectPtr<class UBoxComponent> Box;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UUserWidget> WidgetPtr;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> WidgetClass;

	UPROPERTY(EditInstanceOnly, Category = "Instance")
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	UPROPERTY(EditInstanceOnly, Category = "Instance")
	TObjectPtr<class UAnimationAsset> OpenAnimation;

	UPROPERTY(EditAnywhere, Category = "Interact")
	TObjectPtr<USceneComponent> InteractPos;

	UPROPERTY(EditInstanceOnly, Category = "Instance")
	EGateType CurrentType;

private:
	/* Camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UCameraShakeBase> CameraShakeClass;

};
