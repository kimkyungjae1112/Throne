// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cinematic.generated.h"

UCLASS()
class THRONE_API ACinematic : public AActor
{
	GENERATED_BODY()
	
public:	
	ACinematic();

protected:
	virtual void BeginPlay() override;

public:
    // 시퀀스 플레이어 시작
    UFUNCTION(BlueprintCallable, Category = "Cinematic")
    void PlayCinematic();

    // 시퀀스 플레이어 일시 정지
    UFUNCTION(BlueprintCallable, Category = "Cinematic")
    void PauseCinematic();

    // 시퀀스 플레이어 정지
    UFUNCTION(BlueprintCallable, Category = "Cinematic")
    void StopCinematic();

    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<class UBoxComponent> Box;

    // 시퀀스 애셋을 담는 변수
    UPROPERTY(EditAnywhere, Category = "Cinematic")
    TObjectPtr<class ULevelSequence> LevelSequenceAsset;

    // 시퀀스 플레이어
    UPROPERTY()
    TObjectPtr<class ULevelSequencePlayer> SequencePlayer;

    // 시퀀스 액터
    UPROPERTY()
    class  ALevelSequenceActor* SequenceActor;

};
