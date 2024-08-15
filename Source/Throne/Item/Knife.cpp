// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Knife.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DamageEvents.h"

AKnife::AKnife()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxCollision->SetupAttachment(KnifeMesh);
	BoxCollision->OnComponentHit.AddDynamic(this, &AKnife::OnHit);
	RootComponent = BoxCollision;

	KnifeMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("KnifeMesh"));
	KnifeMesh->SetCollisionProfileName(TEXT("NoCollision"));
	KnifeMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> KnifeMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/DARK_C_KNIGHT/MESHES/WEAPONS/KNIFE/SK_DC_Knight_knife_weapon.SK_DC_Knight_knife_weapon'"));
	if (KnifeMeshRef.Object)
	{
		KnifeMesh->SetSkeletalMesh(KnifeMeshRef.Object);
	}

	PMC = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	PMC->UpdatedComponent = BoxCollision;
	PMC->InitialSpeed = 1200.0f;
	PMC->MaxSpeed = 1200.0f;
	PMC->bRotationFollowsVelocity = true;

}

void AKnife::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerManager;
	GetWorld()->GetTimerManager().SetTimer(TimerManager,
		[&]()
		{
			Destroy();
		}, 10.0f, false);
}

void AKnife::SetDirection(const FVector& InDirection)
{
	PMC->Velocity = InDirection * PMC->InitialSpeed;
}

void AKnife::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		FDamageEvent DamageEvent;
		OtherActor->TakeDamage(100.0f, DamageEvent, GetWorld()->GetFirstPlayerController(), this);
		Destroy();
	}
}




