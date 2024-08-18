// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Bow.h"

// Sets default values
ABow::ABow()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Bow = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bow"));
	RootComponent = Bow;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BowMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/MRPGT/SkeletalMeshes/Bows/SK_Bow_B.SK_Bow_B'"));
	if (BowMeshRef.Object)
	{
		Bow->SetSkeletalMesh(BowMeshRef.Object);
	}
}

// Called when the game starts or when spawned
void ABow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

