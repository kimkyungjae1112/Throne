// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Knife.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AKnife::AKnife()
{
	KnifeMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("KnifeMesh"));
	RootComponent = KnifeMesh;
	
	KnifeMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> KnifeMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/DARK_C_KNIGHT/MESHES/WEAPONS/KNIFE/SK_DC_Knight_knife_weapon.SK_DC_Knight_knife_weapon'"));
	if (KnifeMeshRef.Object)
	{
		KnifeMesh->SetSkeletalMesh(KnifeMeshRef.Object);
	}

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxCollision->SetupAttachment(KnifeMesh);
	BoxCollision->SetCollisionProfileName(TEXT("Knife"));

	PMC = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	PMC->UpdatedComponent = KnifeMesh;
	PMC->InitialSpeed = 1200.0f;
	PMC->MaxSpeed = 1200.0f;
	PMC->bRotationFollowsVelocity = true;

}

void AKnife::BeginPlay()
{
	Super::BeginPlay();

}




