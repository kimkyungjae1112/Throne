// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/Door.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "Interface/DoorInterface.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(Root);
	Box->SetCollisionProfileName(TEXT("ThroneTrigger"));
	Box->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnDoorBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnDoorEndOverlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/DRAGON_CATACOMB/MESHES/GATE/DOOR/SM_Small_door.SM_Small_door'"));
	if (MeshRef.Object)
	{
		Mesh->SetStaticMesh(MeshRef.Object);
	}
	
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassRef(TEXT("/Game/Throne/UI/WBP_Interact.WBP_Interact_C"));
	if (WidgetClassRef.Class)
	{
		WidgetClass = WidgetClassRef.Class;
	}

	CurrentDoorType = EDoorType::Close;
	bDoorFlag = false;


	InitialRotation = Mesh->GetRelativeRotation();
	FrontTargetRotation = InitialRotation;
	FrontTargetRotation.Yaw -= 100.0f;

	BackTargetRotation = InitialRotation;
	BackTargetRotation.Yaw += 100.0f;

}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDoorFlag)
	{
		FRotator CurrentRotation = Mesh->GetRelativeRotation();
		FRotator TargetRotation = CurrentRotation;

		if (Direction.Y > 0)
		{
			if (CurrentDoorType == EDoorType::Close)
			{
				TargetRotation = FMath::RInterpTo(CurrentRotation, FrontTargetRotation, DeltaTime, 2.0f);
				if (FMath::Abs((TargetRotation - FrontTargetRotation).Yaw) < 1.0f)
				{
					TargetRotation = FrontTargetRotation;
					CurrentDoorType = EDoorType::Open;
					bDoorFlag = false; 
				}
			}
			else
			{
				TargetRotation = FMath::RInterpTo(CurrentRotation, InitialRotation, DeltaTime, 2.0f);

				if (FMath::Abs((TargetRotation - InitialRotation).Yaw) < 1.0f)
				{
					TargetRotation = InitialRotation;
					CurrentDoorType = EDoorType::Close;
					bDoorFlag = false; 
				}
			}
		}
		else
		{
			if (CurrentDoorType == EDoorType::Close)
			{
				TargetRotation = FMath::RInterpTo(CurrentRotation, BackTargetRotation, DeltaTime, 2.0f);

				if (FMath::Abs((TargetRotation - BackTargetRotation).Yaw) < 1.0f)
				{
					TargetRotation = BackTargetRotation;
					CurrentDoorType = EDoorType::Open;
					bDoorFlag = false;
				}
			}
			else
			{
				TargetRotation = FMath::RInterpTo(CurrentRotation, InitialRotation, DeltaTime, 2.0f);

				if (FMath::Abs((TargetRotation - InitialRotation).Yaw) < 1.0f)
				{
					TargetRotation = InitialRotation;
					CurrentDoorType = EDoorType::Close;
					bDoorFlag = false; 
				}
			}
		}
		Mesh->SetRelativeRotation(TargetRotation);
	}

}

void ADoor::OnDoorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	WidgetPtr = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	if (WidgetPtr)
	{
		WidgetPtr->AddToViewport();
	}

	IDoorInterface* DoorInterface = Cast<IDoorInterface>(OtherActor);
	if (DoorInterface)
	{
		DoorInterface->SetDoorPointer(this);
	}

	PlayerLocation = OtherActor->GetActorLocation();
	DoorLocation = GetActorLocation();
	Direction = PlayerLocation - DoorLocation;
}

void ADoor::OnDoorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (WidgetPtr->IsInViewport())
	{
		WidgetPtr->RemoveFromViewport();
	}

	bDoorFlag = false;
}

void ADoor::OnTriggerDoor()
{
	if (WidgetPtr == nullptr)
	{
		return;
	}

	if (WidgetPtr->IsInViewport())
	{
		bDoorFlag = true;
	}
}

