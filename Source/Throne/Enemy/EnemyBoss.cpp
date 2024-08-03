// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBoss.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyBoss::AEnemyBoss()
{
	//Capsule
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	//Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/DF_DRAGON_KNIGHT/MESHES/UE5/SK_Dragon_knight_UE5.SK_Dragon_knight_UE5'"));
	if (MeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	/* Item */
	Sword = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Sword"));
	Sword->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SwordMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/DF_DRAGON_KNIGHT/MESHES/SWORD/SK_Dragon_knight_sword.SK_Dragon_knight_sword'"));
	if (SwordMeshRef.Object)
	{
		Sword->SetSkeletalMesh(SwordMeshRef.Object);
	}


}

void AEnemyBoss::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyBoss::AttackByAI(UAnimMontage* InAnimMontage)
{
	Super::AttackByAI(DefaultAttackMontage);
}
