// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ThroneCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Component/AbilityComponent.h"
#include "Component/CharacterStatComponent.h"
#include "UI/HUDWidget.h"
#include "Engine/OverlapResult.h"
#include "Engine/DamageEvents.h"

AThroneCharacter::AThroneCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	/* Capsule */
	GetCapsuleComponent()->SetCapsuleSize(42.0f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	/* Mesh */
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/DARK_C_KNIGHT/MESHES/UE4/SK_DC_Knight_UE4_full_Silver.SK_DC_Knight_UE4_full_Silver'"));
	if (MeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	
	/* Character Movement */
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	/* Pawn */
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	/* Camera */
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	/* Input */
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Throne/Input/IMC/IMC_Default.IMC_Default'"));
	if (IMCRef.Object)
	{
		IMC = IMCRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Throne/Input/Action/IA_Move.IA_Move'"));
	if (MoveActionRef.Object)
	{
		MoveAction = MoveActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> LookUpActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Throne/Input/Action/IA_LookUp.IA_LookUp'"));
	if (LookUpActionRef.Object)
	{
		LookUpAction = LookUpActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Throne/Input/Action/IA_Jump.IA_Jump'"));
	if (JumpActionRef.Object)
	{
		JumpAction = JumpActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> DefaultAttackActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Throne/Input/Action/IA_DefaultAttack.IA_DefaultAttack'"));
	if (DefaultAttackActionRef.Object)
	{
		DefaultAttackAction = DefaultAttackActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> DefendActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Throne/Input/Action/IA_Defend.IA_Defend'"));
	if (DefendActionRef.Object)
	{
		DefendAction = DefendActionRef.Object;
	}

	/* Item */
	Sword = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Swoard"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SwoardMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/DARK_C_KNIGHT/MESHES/WEAPONS/SWORD/SK_DC_Knight_Sword.SK_DC_Knight_Sword'"));
	if (SwoardMeshRef.Object)
	{
		Sword->SetSkeletalMesh(SwoardMeshRef.Object);
	}
	Sword->SetupAttachment(GetMesh(), TEXT("weapon_r"));

	Shield = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shiled"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ShieldMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/DARK_C_KNIGHT/MESHES/WEAPONS/SHIELD/SK_DC_KnightShield_left_hand.SK_DC_KnightShield_left_hand'"));
	if (ShieldMeshRef.Object)
	{
		Shield->SetSkeletalMesh(ShieldMeshRef.Object);
	}
	Shield->SetupAttachment(GetMesh(), TEXT("shield_l"));
	
	/* Components */
	Ability = CreateDefaultSubobject<UAbilityComponent>(TEXT("Ability Component"));
	Stat = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("Stat Component"));

}

void AThroneCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(IMC, 0);
	}
	
	/* Delegate */
	Stat->OnHpZero.AddUObject(this, &AThroneCharacter::Death);
	Ability->OnDefaultAttackUseEnergy.BindUObject(this, &AThroneCharacter::DefaultAttackUseEnergy);
}

void AThroneCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AThroneCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AThroneCharacter::Move);
	EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &AThroneCharacter::LookUp);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(DefaultAttackAction, ETriggerEvent::Started, this, &AThroneCharacter::DefaultAttack);
	EnhancedInputComponent->BindAction(DefendAction, ETriggerEvent::Started, this, &AThroneCharacter::Defend);

}

float AThroneCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Stat->ApplyDamage(Damage);

	return Damage;
}

/************* Interface *************/
void AThroneCharacter::SetHUD(UHUDWidget* InHUDWidget)
{
	HUDWidget = InHUDWidget;
	if (HUDWidget)
	{
		HUDWidget->SetMaxHp(Stat->GetMaxHp());
		HUDWidget->SetMaxEnergy(Stat->GetMaxEnergy());
		HUDWidget->UpdateHpBar(Stat->GetMaxHp());
		HUDWidget->UpdateEnergyBar(Stat->GetMaxEnergy());
		Stat->OnHpChanged.AddUObject(HUDWidget, &UHUDWidget::UpdateHpBar);
		Stat->OnEnergyChanged.AddUObject(HUDWidget, &UHUDWidget::UpdateEnergyBar);
	}
}

void AThroneCharacter::Move(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, InputValue.X);
	AddMovementInput(RightDirection, InputValue.Y);
}

/************* Input *************/
void AThroneCharacter::LookUp(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	AddControllerPitchInput(InputValue.X);
	AddControllerYawInput(-InputValue.Y);
}

void AThroneCharacter::DefaultAttack()
{
	Ability->BeginComboAttack();
}

void AThroneCharacter::Defend()
{
	UE_LOG(LogTemp, Display, TEXT("Defend"));
	Ability->BeginDefend();
}

void AThroneCharacter::Death()
{
}

void AThroneCharacter::DefaultAttackUseEnergy(float UseEnergy)
{
	Stat->SetEnergy(UseEnergy);
}

