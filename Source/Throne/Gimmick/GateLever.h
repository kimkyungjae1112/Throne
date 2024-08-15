// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/GateLeverDelegateInterface.h"
#include "GateLever.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnGateLeverOpen)
DECLARE_MULTICAST_DELEGATE(FOnGateLeverClose)

UENUM(BlueprintType)
enum class ELeverType : uint8
{
	Close = 0,
	Open
};

UCLASS()
class THRONE_API AGateLever : public AActor
{
	GENERATED_BODY()
	
public:	
	AGateLever();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


public:
	static FOnGateLeverOpen OnGateLeverOpen;
	static FOnGateLeverClose OnGateLeverClose;

	FORCEINLINE ELeverType GetLeverType() const { return CurrentLeverType; }
	FORCEINLINE bool GetGateLeverFlag() const { return bGateLever; }
	FORCEINLINE FVector GetInteractLocation() const { return InteractPos->GetComponentLocation(); }
	FORCEINLINE FRotator GetInteractRotation() const { return InteractPos->GetComponentRotation(); }

	UFUNCTION()
	void OnLeverTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnLeverTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	void OnTriggerGateLever();

private:
	UPROPERTY(VisibleAnywhere, Category = "GateLevel")
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = "GateLevel")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, Category = "GateLevel")
	TObjectPtr<class UBoxComponent> BoxCollision;

	UPROPERTY(VisibleAnywhere, Category = "GateLevel")
	TObjectPtr<class UNiagaraComponent> NiagaraComponent;

	UPROPERTY(VisibleAnywhere, Category = "GateLevel")
	TObjectPtr<class UNiagaraSystem> NiagaraAsset;
	
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UUserWidget> WidgetPtr;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> WidgetClass;

	UPROPERTY(EditAnywhere, Category = "GateLevel")
	TObjectPtr<USceneComponent> InteractPos;

	ELeverType CurrentLeverType;

private:
	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<class UAnimationAsset> OpenAnimation;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<class UAnimationAsset> CloseAnimation;

	bool bGateLever;

};
