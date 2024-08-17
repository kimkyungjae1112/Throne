// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/Ladder.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "GimmickInterface/LadderInterface.h"

ALadder::ALadder()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	LadderBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LadderBody"));
	LadderBody->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> LadderBodyRef(TEXT("/Script/Engine.StaticMesh'/Game/DRAGON_CATACOMB/MESHES/OTHER/LADDER/SM_Ladder_bar_2m.SM_Ladder_bar_2m'"));
	if (LadderBodyRef.Object)
	{
		LadderBody->SetStaticMesh(LadderBodyRef.Object);
	}

	LadderKnob_A = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LadderKnob A"));
	LadderKnob_A->SetupAttachment(LadderBody);

	LadderKnob_B = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LadderKnob B"));
	LadderKnob_B->SetupAttachment(LadderBody);

	LadderKnob_C = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LadderKnob C"));
	LadderKnob_C->SetupAttachment(LadderBody);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> LadderKnobRef(TEXT("/Script/Engine.StaticMesh'/Game/DRAGON_CATACOMB/MESHES/OTHER/LADDER/SM_ladder_bar_a.SM_ladder_bar_a'"));
	if (LadderKnobRef.Object)
	{
		LadderKnob_A->SetStaticMesh(LadderKnobRef.Object);
		LadderKnob_B->SetStaticMesh(LadderKnobRef.Object);
		LadderKnob_C->SetStaticMesh(LadderKnobRef.Object);
	}

	TopBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Top Box"));
	TopBox->SetupAttachment(Root);
	TopBox->SetCollisionProfileName(TEXT("ThroneTrigger"));
	TopBox->OnComponentBeginOverlap.AddDynamic(this, &ALadder::OnLadderBeginOverlap);
	TopBox->OnComponentEndOverlap.AddDynamic(this, &ALadder::OnLadderEndOverlap);

	BottomBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Bottom Box"));
	BottomBox->SetupAttachment(Root);
	BottomBox->SetCollisionProfileName(TEXT("ThroneTrigger"));
	BottomBox->OnComponentBeginOverlap.AddDynamic(this, &ALadder::OnLadderBeginOverlap);
	BottomBox->OnComponentEndOverlap.AddDynamic(this, &ALadder::OnLadderEndOverlap);

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassRef(TEXT("/Game/Throne/UI/WBP_Interact.WBP_Interact_C"));
	if (WidgetClassRef.Class)
	{
		WidgetClass = WidgetClassRef.Class;
	}

}

void ALadder::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALadder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALadder::OnLadderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	WidgetPtr = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	if (WidgetPtr)
	{
		WidgetPtr->AddToViewport();
	}

	ILadderInterface* Interface = Cast<ILadderInterface>(OtherActor);
	if (Interface)
	{
		Interface->SetLadder(this);
	}
}

void ALadder::OnLadderEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (WidgetPtr->IsInViewport())
	{
		WidgetPtr->RemoveFromViewport();
	}

	ILadderInterface* Interface = Cast<ILadderInterface>(OtherActor);
	if (Interface)
	{
		Interface->SetLadder(nullptr);
	}
}

void ALadder::OnLadderClimb()
{
}

