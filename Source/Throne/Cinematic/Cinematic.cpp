// Fill out your copyright notice in the Description page of Project Settings.


#include "Cinematic/Cinematic.h"
#include "Components/BoxComponent.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Kismet/GameplayStatics.h"

ACinematic::ACinematic()
{
	PrimaryActorTick.bCanEverTick = true;

    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    RootComponent = Box;
    Box->SetCollisionProfileName(TEXT("ThroneTrigger"));
    Box->OnComponentBeginOverlap.AddDynamic(this, &ACinematic::OnOverlap);
}

void ACinematic::BeginPlay()
{
	Super::BeginPlay();
	
    if (LevelSequenceAsset)
    {
        // 시퀀스 플레이어와 액터 생성
        FMovieSceneSequencePlaybackSettings PlaybackSettings;
        SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequenceAsset, PlaybackSettings, SequenceActor);

        if (SequencePlayer)
        {
            UE_LOG(LogTemp, Warning, TEXT("Sequence Player Created!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("LevelSequenceAsset is not set!"));
    }
}

void ACinematic::PlayCinematic()
{
    if (SequencePlayer)
    {
        SequencePlayer->Play();
        UE_LOG(LogTemp, Warning, TEXT("Cinematic Started"));
    }
}

void ACinematic::PauseCinematic()
{
    if (SequencePlayer)
    {
        SequencePlayer->Pause();
        UE_LOG(LogTemp, Warning, TEXT("Cinematic Paused"));
    }
}

void ACinematic::StopCinematic()
{
    if (SequencePlayer)
    {
        SequencePlayer->Stop();
        UE_LOG(LogTemp, Warning, TEXT("Cinematic Stopped"));
    }
}

void ACinematic::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    PlayCinematic();
}


