// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Arrow.h"
#include "Components/BoxComponent.h"

AArrow::AArrow()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	
	Arrow = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Box);
	Arrow->SetCollisionProfileName(TEXT("NoCollision"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ArrowMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/SKnight_modular/Skeleton_Knight_07/mesh/weapon/SK_Arrow.SK_Arrow'"));
	if (ArrowMeshRef.Object)
	{
		Arrow->SetSkeletalMesh(ArrowMeshRef.Object);
	}

}

void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

