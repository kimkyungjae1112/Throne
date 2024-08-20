// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Bow.h"

// Sets default values
ABow::ABow()
{
	PrimaryActorTick.bCanEverTick = true;

	Bow = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bow"));
	RootComponent = Bow;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BowMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/MRPGT/SkeletalMeshes/Bows/SK_Bow_B.SK_Bow_B'"));
	if (BowMeshRef.Object)
	{
		Bow->SetSkeletalMesh(BowMeshRef.Object);
	}
}

void ABow::BeginPlay()
{
	Super::BeginPlay();
	
}
