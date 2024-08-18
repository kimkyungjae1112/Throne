// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyArcher.h"
#include "Components/CapsuleComponent.h"
#include "Component/EnemyStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	Bow = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Bow->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BowMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/MRPGT/SkeletalMeshes/Bows/SK_Bow_B.SK_Bow_B'"));
	if (BowMeshRef.Object)
	{
		Bow->SetSkeletalMesh(BowMeshRef.Object);
	}
	
	/* Capsule */
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	/* Component */
	Stat = CreateDefaultSubobject<UEnemyStatComponent>(TEXT("Stat"));

	/* Character Movement */
	GetCharacterMovement()->MaxWalkSpeed = Stat->GetMoveSpeed();
	
}
