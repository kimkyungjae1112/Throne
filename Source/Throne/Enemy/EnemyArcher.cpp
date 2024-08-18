// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyArcher.h"
#include "Components/CapsuleComponent.h"
#include "Component/EnemyStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Item/Bow.h"

AEnemyArcher::AEnemyArcher()
{
	
	/* Mesh */
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/SKnight_modular/Skeleton_Knight_06/mesh/SK_SKnight_full_06.SK_SKnight_full_06'"));
	if (BodyMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(BodyMeshRef.Object);
	}

	/* Capsule */
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	/* Component */
	Stat = CreateDefaultSubobject<UEnemyStatComponent>(TEXT("Stat"));
	Stat->OnHpZero.AddUObject(this, &AEnemyArcher::SetDead);

	/* Character Movement */
	GetCharacterMovement()->MaxWalkSpeed = Stat->GetMoveSpeed();
	
}

void AEnemyArcher::BeginPlay()
{
	Super::BeginPlay();

	Bow = GetWorld()->SpawnActor<ABow>(BowClass);
	Bow->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_lSocket"));
	Bow->SetOwner(this);
}

float AEnemyArcher::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	Stat->ApplyDamage(Damage);
	
	return Damage;
}

void AEnemyArcher::SetDead()
{
}
