// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/DragonGate.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "GimmickInterface/DragonGateInterface.h"
#include "Camera/CameraShakeBase.h"

ADragonGate::ADragonGate()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(Root);
	Box->SetCollisionProfileName(TEXT("ThroneTrigger"));
	Box->OnComponentBeginOverlap.AddDynamic(this, &ADragonGate::OnDragonGateBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ADragonGate::OnDragonGateEndOverlap);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionProfileName(TEXT("BlockAll"));

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassRef(TEXT("/Game/Throne/UI/WBP_Interact.WBP_Interact_C"));
	if (WidgetClassRef.Class)
	{
		WidgetClass = WidgetClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UCameraShakeBase> CameraShakeClassRef(TEXT("/Game/DRAGON_CATACOMB/BLUEPRINTS/INTERACTIVE/BP_Camera_shake_basic.BP_Camera_shake_basic_C"));
	if (CameraShakeClassRef.Class)
	{
		CameraShakeClass = CameraShakeClassRef.Class;
	}

	InteractPos = CreateDefaultSubobject<USceneComponent>(TEXT("Interact Pos"));
	InteractPos->SetupAttachment(Root);
}

void ADragonGate::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADragonGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADragonGate::OnDragonGateBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	WidgetPtr = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	if (WidgetPtr)
	{
		WidgetPtr->AddToViewport();
	}

	IDragonGateInterface* Interface = Cast<IDragonGateInterface>(OtherActor);
	if (Interface)
	{
		Interface->SetDragonGate(this);
	}
}

void ADragonGate::OnDragonGateEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (WidgetPtr->IsInViewport())
	{
		WidgetPtr->RemoveFromViewport(); 
	}

	IDragonGateInterface* Interface = Cast<IDragonGateInterface>(OtherActor);
	if (Interface)
	{
		Interface->SetDragonGate(nullptr);
	}
}

void ADragonGate::OnDragonGateTrigger()
{
	if (!WidgetPtr->IsInViewport())
	{
		return;
	}

	Mesh->PlayAnimation(OpenAnimation, false);
	GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(CameraShakeClass);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,
		[&]()
		{
			GetWorld()->GetFirstPlayerController()->ClientStopCameraShake(CameraShakeClass);
		}, 3.0f, false);
}

