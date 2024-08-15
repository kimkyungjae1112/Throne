// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/GateLever.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Blueprint/UserWidget.h"

FOnGateLeverOpen AGateLever::OnGateLeverOpen;
FOnGateLeverClose AGateLever::OnGateLeverClose;

AGateLever::AGateLever()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxCollision->SetupAttachment(Root);
	BoxCollision->SetCollisionProfileName(TEXT("ThroneTrigger"));
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AGateLever::OnLeverTriggerBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AGateLever::OnLeverTriggerEndOverlap);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/DRAGON_CATACOMB/MESHES/GATE/DRAGON_LEVER/SK_Dragon_lever.SK_Dragon_lever'"));
	if (MeshRef.Object)
	{
		Mesh->SetSkeletalMesh(MeshRef.Object);
	}
	
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	NiagaraComponent->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraAssetRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/DRAGON_CATACOMB/PARTICLES/NS_Fire_yellow_horizontal.NS_Fire_yellow_horizontal'"));
	if (NiagaraAssetRef.Object)
	{
		NiagaraAsset = NiagaraAssetRef.Object;
	}

	NiagaraComponent->SetAsset(NiagaraAsset);
	NiagaraComponent->bAutoActivate = false;

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassRef(TEXT("/Game/Throne/UI/WBP_Interact.WBP_Interact_C"));
	if (WidgetClassRef.Class)
	{
		WidgetClass = WidgetClassRef.Class;
	}

	InteractPos = CreateDefaultSubobject<USceneComponent>(TEXT("Interact"));
	InteractPos->SetupAttachment(Root);

	CurrentLeverType = ELeverType::Close;
	bGateLever = false;
}

void AGateLever::BeginPlay()
{
	Super::BeginPlay();

}

void AGateLever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AGateLever::OnTriggerGateLever()
{
	if (WidgetPtr != nullptr)
	{
		if (WidgetPtr->IsInViewport() && CurrentLeverType == ELeverType::Close)
		{
			Mesh->PlayAnimation(OpenAnimation, false);
			NiagaraComponent->SetActive(true);

			CurrentLeverType = ELeverType::Open;
			OnGateLeverOpen.Broadcast();
		}
		else if (WidgetPtr->IsInViewport() && CurrentLeverType == ELeverType::Open)
		{
			Mesh->PlayAnimation(CloseAnimation, false);
			NiagaraComponent->SetActive(false);

			CurrentLeverType = ELeverType::Close;
			OnGateLeverClose.Broadcast();
		}
	}
}

void AGateLever::OnLeverTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	WidgetPtr = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	if (WidgetPtr)
	{
		WidgetPtr->AddToViewport();
		bGateLever = true;
	}

	IGateLeverDelegateInterface* Interface = Cast<IGateLeverDelegateInterface>(OtherActor);
	if (Interface)
	{
		Interface->SetGateLever(this);
	}
}

void AGateLever::OnLeverTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (WidgetPtr->IsInViewport())
	{
		WidgetPtr->RemoveFromViewport();
		bGateLever = false;
	}
}

