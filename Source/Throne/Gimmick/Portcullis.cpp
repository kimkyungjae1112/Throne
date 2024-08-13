// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/Portcullis.h"
#include "Gimmick/GateLever.h"
#include "Camera/CameraShakeBase.h"

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

	static ConstructorHelpers::FClassFinder<UCameraShakeBase> CameraShakeClassRef(TEXT("/Game/DRAGON_CATACOMB/BLUEPRINTS/INTERACTIVE/BP_Camera_shake_basic.BP_Camera_shake_basic_C"));
	if (CameraShakeClassRef.Class)
	{
		CameraShakeClass = CameraShakeClassRef.Class;
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
			GetWorld()->GetFirstPlayerController()->ClientStopCameraShake(CameraShakeClass);
		}
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
		}
	}
}

void APortcullis::Open()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,
		[&]()
		{
			bOpenFlag = true;
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(CameraShakeClass);
		}, 3.5f, false);
}

void APortcullis::Close()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,
		[&]()
		{
			bCloseFlag = true;
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(CameraShakeClass);
		}, 3.5f, false);
}

