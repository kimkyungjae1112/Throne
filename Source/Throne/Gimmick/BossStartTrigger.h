// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossStartTrigger.generated.h"

DECLARE_DELEGATE(FInBossRoom)

UCLASS()
class THRONE_API ABossStartTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	ABossStartTrigger();

	static FInBossRoom InBossRoom;

	UFUNCTION()
	void OnBossRoomBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> Box;

};
