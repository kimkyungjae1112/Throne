// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Arrow.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DamageEvents.h"

AArrow::AArrow()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->OnComponentHit.AddDynamic(this, &AArrow::OnHit);

	Arrow = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Box);
	Arrow->SetCollisionProfileName(TEXT("NoCollision"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ArrowMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/SKnight_modular/Skeleton_Knight_07/mesh/weapon/SK_Arrow.SK_Arrow'"));
	if (ArrowMeshRef.Object)
	{
		Arrow->SetSkeletalMesh(ArrowMeshRef.Object);
	}

	PMC = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	PMC->UpdatedComponent = Box;
	PMC->InitialSpeed = 0.0f;
	PMC->MaxSpeed = 0.0f;

	DelayedTime = 3.0f;
	Direction = FVector::ZeroVector;
}

void AArrow::BeginPlay()
{
	Super::BeginPlay();
}

void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArrow::SetDirection(const FVector& InDirection)
{
	Direction = InDirection;
}

void AArrow::ActiveMovement()
{
	PMC->InitialSpeed = 2000.0f;
	PMC->MaxSpeed = 2000.0f;
	PMC->Velocity = Direction * PMC->InitialSpeed;
}

void AArrow::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == this)
	{
		Destroy();
	}

	if (OtherActor && OtherActor != this)
	{
		FDamageEvent DamageEvent;
		OtherActor->TakeDamage(1000.0f, DamageEvent, GetWorld()->GetFirstPlayerController(), this);
	}
}


