// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyArcher.h"
#include "Components/CapsuleComponent.h"
#include "Component/EnemyStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Item/Bow.h"
#include "Item/Arrow.h"
#include "Animation/AnimInstance.h"
#include "AI/ArcherAIController.h"

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
	
	ArrowPos = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArrowPos"));
	ArrowPos->SetupAttachment(GetMesh());
	ArrowPos->SetRelativeScale3D(FVector(1.0f, 3.0f, 1.0f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ArrowMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/SKnight_modular/Skeleton_Knight_07/mesh/weapon/SK_Arrow.SK_Arrow'"));
	if (ArrowMeshRef.Object)
	{
		ArrowMesh = ArrowMeshRef.Object;
	}
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
	BeginHitReaction();
	Stat->ApplyDamage(Damage);
	
	return Damage;
}

void AEnemyArcher::SetAimingDelegate(FOnAimingFinished OnAimingFinished)
{
	AimingFinished = OnAimingFinished;
}

void AEnemyArcher::AimingByArcher()
{
	BeginAiming();
}

//Notify, ArcherInterface 로 연결 
void AEnemyArcher::ArrowFire()
{
	SpawnArrow();
	BeginStringLay();

	DetachmentArrow();
}

void AEnemyArcher::DetachmentArrow()
{
	ArrowPos->SetSkeletalMesh(nullptr);
	ArrowPos->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void AEnemyArcher::BeginAiming()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(AimMontage);
		BeginStringPull();

		ArrowPos->SetSkeletalMesh(ArrowMesh);
		ArrowPos->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rSocket"));

		FOnMontageEnded MontageEnded;
		MontageEnded.BindUObject(this, &AEnemyArcher::EndAiming);
		AnimInstance->Montage_SetEndDelegate(MontageEnded, AimMontage);
	}
}

void AEnemyArcher::EndAiming(UAnimMontage* Target, bool IsProperlyEnded)
{
	AimingFinished.ExecuteIfBound();
}

void AEnemyArcher::BeginStringPull()
{
	UAnimInstance* AnimInstance = Bow->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(BowStringPullMontage);
	}
}

void AEnemyArcher::BeginStringLay()
{
	UAnimInstance* AnimInstance = Bow->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(1.0f);
	}
}

void AEnemyArcher::SetDead()
{
	AArcherAIController* AIController = Cast<AArcherAIController>(GetController());
	if (AIController)
	{
		AIController->StopAI();
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(DeadMontage);
		SetActorEnableCollision(false);
	}

	FTimerHandle DestroyTimer;
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer,
		[&]()
		{
			Destroy();
			Bow->Destroy();
		}, 4.0f, false);
}

void AEnemyArcher::BeginHitReaction()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(HitReactionMontage);
	}
}

void AEnemyArcher::SpawnArrow()
{
	FVector SpawnLocation = GetMesh()->GetSocketLocation(TEXT("hand_rSocket"));
	FRotator SpawnRotation = GetMesh()->GetSocketRotation(TEXT("hand_rSocket"));

	Arrow = GetWorld()->SpawnActor<AArrow>(ArrowClass, SpawnLocation, SpawnRotation);
	Arrow->SetDirection(GetActorForwardVector());
	Arrow->ActiveMovement();
	Arrow->SetOwner(this);
}
