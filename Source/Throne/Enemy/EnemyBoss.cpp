// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBoss.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyBoss::AEnemyBoss()
{
	//Capsule
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	//Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/DF_DRAGON_KNIGHT/MESHES/UE5/SK_Dragon_knight_UE5.SK_Dragon_knight_UE5'"));
	if (MeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}
	GetMesh()->SetCollisionProfileName(TEXT("Pawn"));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));


}

void AEnemyBoss::BeginPlay()
{
	Super::BeginPlay();
}
