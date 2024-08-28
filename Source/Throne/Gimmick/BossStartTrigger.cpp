// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/BossStartTrigger.h"
#include "Components/BoxComponent.h"

FInBossRoom ABossStartTrigger::InBossRoom;

ABossStartTrigger::ABossStartTrigger()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;

	Box->SetCollisionProfileName(TEXT("ThroneTrigger"));
	Box->OnComponentBeginOverlap.AddDynamic(this, &ABossStartTrigger::OnBossRoomBeginOverlap);
}

void ABossStartTrigger::OnBossRoomBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	InBossRoom.ExecuteIfBound();
}



