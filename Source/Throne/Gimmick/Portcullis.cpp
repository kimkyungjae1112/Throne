// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/Portcullis.h"
#include "Gimmick/GateLever.h"

APortcullis::APortcullis()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/DRAGON_CATACOMB/MESHES/GATE/PORTCULLIS/SM_Portcullis.SM_Portcullis'"));
	if (MeshRef.Object)
	{
		Mesh->SetStaticMesh(MeshRef.Object);
	}

	bOpenFlag = false;
	bCloseFlag = false;
}

void APortcullis::BeginPlay()
{
	Super::BeginPlay();

	AGateLever::OnGateLeverOpen.AddUObject(this, &APortcullis::Open);
	AGateLever::OnGateLeverClose.AddUObject(this, &APortcullis::Close);

	Start = GetActorLocation();
	MoveTime = 4.0f;
}

void APortcullis::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOpenFlag)
	{		
		FVector Current = GetActorLocation();
		FVector Target = Start + FVector(0.0f, 0.0f, 400.0f);
		float Speed = FVector::Distance(Start, Target) / MoveTime;

		FVector NewLocation = FMath::VInterpConstantTo(Current, Target, DeltaTime, Speed);
		SetActorLocation(NewLocation);

		if (Current == Target)
		{
			bOpenFlag = false;
			UE_LOG(LogTemp, Display, TEXT("False"));
		}
		UE_LOG(LogTemp, Display, TEXT("True"));
	}
	if (bCloseFlag)
	{
		FVector Current = GetActorLocation();
		FVector Target = Start + FVector(0.0f, 0.0f, -400.0f);
		float Speed = FVector::Distance(Start, Target) / MoveTime;

		FVector NewLocation = FMath::VInterpConstantTo(Current, Target, DeltaTime, Speed);
		SetActorLocation(NewLocation);

		if (Current == Target)
		{
			bCloseFlag = false;
			UE_LOG(LogTemp, Display, TEXT("False"));
		}
		UE_LOG(LogTemp, Display, TEXT("True"));
	}
}

void APortcullis::Open()
{
	UE_LOG(LogTemp, Display, TEXT("UP"));
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,
		[&]()
		{
			bOpenFlag = true;
		}, 3.0f, false);
}

void APortcullis::Close()
{
	UE_LOG(LogTemp, Display, TEXT("DOWNDOWN"));
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,
		[&]()
		{
			bCloseFlag = true;
		}, 3.0f, false);
}

