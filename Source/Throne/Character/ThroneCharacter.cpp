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
#include "Player/ThronePlayerController.h"
#include "Item/ItemData.h"
#include "Animation/CharacterAnimInstance.h"
#include "Character/CharacterControlData.h"
#include "Character/CharacterAimKnifeData.h"
#include "Item/Knife.h"
#include "Gimmick/GateLever.h"
#include "Gimmick/Door.h"
#include "Gimmick/DragonGate.h"
#include "Gimmick/Ladder.h"

AThroneCharacter::AThroneCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	/* Components */
	Ability = CreateDefaultSubobject<UAbilityComponent>(TEXT("Ability Component"));
	Stat = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("Stat Component"));

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
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	CurrentCharacterMode = ECharacterMode::Default;

	/* Character Movement */
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->MaxWalkSpeed = Stat->GetRunMoveSpeed();

	/* Pawn */
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	/* Camera */
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	/* Input */
	static ConstructorHelpers::FObjectFinder<UCharacterControlData> DefaultModeRef(TEXT("/Script/Throne.CharacterControlData'/Game/Throne/Character/Data/DA_DefaultControl.DA_DefaultControl'"));
	if (DefaultModeRef.Object)
	{
		CharacterControlManager.Add(ECharacterMode::Default, DefaultModeRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UCharacterControlData> HoldWeaponModeRef(TEXT("/Script/Throne.CharacterControlData'/Game/Throne/Character/Data/DA_HoldWeaponControl.DA_HoldWeaponControl'"));
	if (DefaultModeRef.Object)
	{
		CharacterControlManager.Add(ECharacterMode::HoldWeapon, HoldWeaponModeRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UCharacterControlData> LadderModeRef(TEXT("/Script/Throne.CharacterControlData'/Game/Throne/Character/Data/DA_LadderControl.DA_LadderControl'"));
	if (LadderModeRef.Object)
	{
		CharacterControlManager.Add(ECharacterMode::Ladder, LadderModeRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Throne/Input/Action/IA_Move.IA_Move'"));
	if (MoveActionRef.Object)
	{
		MoveAction = MoveActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveLadderActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Throne/Input/Action/IA_MoveLadder.IA_MoveLadder'"));
	if (MoveLadderActionRef.Object)
	{
		MoveLadderAction = MoveLadderActionRef.Object;
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
	static ConstructorHelpers::FObjectFinder<UInputAction> InteractActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Throne/Input/Action/IA_Interact.IA_Interact'"));
	if (InteractActionRef.Object)
	{
		InteractAction = InteractActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> SheathActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Throne/Input/Action/IA_Sheath.IA_Sheath'"));
	if (SheathActionRef.Object)
	{
		SheathAction = SheathActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> AimKnifeActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Throne/Input/Action/IA_AimKnife.IA_AimKnife'"));
	if (AimKnifeActionRef.Object)
	{
		AimKnifeAction = AimKnifeActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> FireKnifeActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Throne/Input/Action/IA_FireKnife.IA_FireKnife'"));
	if (FireKnifeActionRef.Object)
	{
		FireKnifeAction = FireKnifeActionRef.Object;
	}

	/* Data Asset */
	static ConstructorHelpers::FObjectFinder<UCharacterAimKnifeData> AimKnifeDataRef(TEXT("/Script/Throne.CharacterAimKnifeData'/Game/Throne/Character/Data/DA_AimKnife.DA_AimKnife'"));
	if (AimKnifeDataRef.Object)
	{
		AimKnifeDatas.Add(AimKnifeDataRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UCharacterAimKnifeData> NonAimKnifeDataRef(TEXT("/Script/Throne.CharacterAimKnifeData'/Game/Throne/Character/Data/DA_NonAimKnife.DA_NonAimKnife'"));
	if (NonAimKnifeDataRef.Object)
	{
		AimKnifeDatas.Add(NonAimKnifeDataRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UItemData> ItemDataRef(TEXT("/Script/Throne.ItemData'/Game/Throne/Item/DA_Item.DA_Item'"));
	if (ItemDataRef.Object)
	{
		ItemData = ItemDataRef.Object;
	}
	
	/* Item */
	Sword = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Swoard"));
	Sword->SetupAttachment(GetMesh(), TEXT("weapon_r"));
	Sword->SetCollisionProfileName(TEXT("ThroneTrigger"));

	Shield = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shiled"));
	Shield->SetupAttachment(GetMesh(), TEXT("shield_l"));

	Knife = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Knife"));
	Knife->SetupAttachment(GetMesh());

	static ConstructorHelpers::FClassFinder<AKnife> KnifeClassRef(TEXT("/Game/Throne/Blueprints/BP_Knife.BP_Knife_C"));
	if (KnifeClassRef.Class)
	{
		KnifeClass = KnifeClassRef.Class;
	}
}

void AThroneCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetCharacterControl(CurrentCharacterMode);

	Ability->SetPlayerController(GetPlayerController());

	/* Delegate */
	Stat->OnHpZero.AddUObject(this, &AThroneCharacter::Death);
	Ability->OnOutSheath.BindUObject(this, &AThroneCharacter::AttachWeaponHand);
	Ability->OnInSheath.BindUObject(this, &AThroneCharacter::AttachWeaponSheath);
	Ability->OnDefaultAttackUseEnergy.BindUObject(Stat, &UCharacterStatComponent::SetEnergy);
	Ability->OnJumpAttackUseEnergy.BindUObject(Stat, &UCharacterStatComponent::SetEnergy);
}

void AThroneCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AThroneCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveLadderAction, ETriggerEvent::Triggered, this, &AThroneCharacter::MoveLadder);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AThroneCharacter::Move);
	EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &AThroneCharacter::LookUp);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(DefaultAttackAction, ETriggerEvent::Started, this, &AThroneCharacter::DefaultAttack);
	EnhancedInputComponent->BindAction(DefendAction, ETriggerEvent::Triggered, this, &AThroneCharacter::BeginDefend);
	EnhancedInputComponent->BindAction(DefendAction, ETriggerEvent::Completed, this, &AThroneCharacter::EndDefend);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AThroneCharacter::AcquisitionItem);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AThroneCharacter::GateLeverInteract);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AThroneCharacter::DoorInteract);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AThroneCharacter::DragonGateInteract);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AThroneCharacter::LadderInteract);
	EnhancedInputComponent->BindAction(SheathAction, ETriggerEvent::Started, this, &AThroneCharacter::Sheath);
	EnhancedInputComponent->BindAction(AimKnifeAction, ETriggerEvent::Started, this, &AThroneCharacter::BeginAimKnife);
	EnhancedInputComponent->BindAction(AimKnifeAction, ETriggerEvent::Completed, this, &AThroneCharacter::EndAimKnife);
	EnhancedInputComponent->BindAction(FireKnifeAction, ETriggerEvent::Started, this, &AThroneCharacter::FireKnife);

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

void AThroneCharacter::BeginOverlapTakeItem(UItemData* InItemData)
{
	ItemData = InItemData;
	GetPlayerController()->DisplayItemInteract();
}

void AThroneCharacter::EndOverlapTakeItem()
{
	GetPlayerController()->HideItemInteract();
}

void AThroneCharacter::SetGateLever(AGateLever* InGateLever)
{
	GateLever = InGateLever;
}

void AThroneCharacter::SetDoorPointer(ADoor* InDoor)
{
	Door = InDoor;
}

void AThroneCharacter::SetDragonGate(ADragonGate* InDragonGate)
{
	DragonGate = InDragonGate;
}

void AThroneCharacter::SetLadder(ALadder* InLadder)
{
	Ladder = InLadder;
}

/************* Input *************/
void AThroneCharacter::ChangeCharacterControl()
{
	if (CurrentCharacterMode == ECharacterMode::Default)
	{
		SetCharacterControl(ECharacterMode::HoldWeapon);
	}
	else if (CurrentCharacterMode == ECharacterMode::HoldWeapon)
	{
		SetCharacterControl(ECharacterMode::Default);
	}
}

void AThroneCharacter::SetCharacterControl(ECharacterMode InCharacterMode)
{
	UCharacterControlData* ControlData = CharacterControlManager[InCharacterMode];
	ensure(ControlData);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* MappingContext = ControlData->InputMappingContext;
		if (MappingContext)
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}

	CurrentCharacterMode = InCharacterMode;
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

void AThroneCharacter::MoveLadder(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	FVector Direction = GetActorUpVector() * InputValue;


	AddMovementInput(Direction);
}

void AThroneCharacter::LookUp(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	AddControllerPitchInput(InputValue.X);
	AddControllerYawInput(-InputValue.Y);
}

void AThroneCharacter::DefaultAttack()
{
	if (!GetCharacterMovement()->IsFalling() && Stat->GetCurrentEnergy() >= Ability->DefaultAttackUseEnergy)
	{
		Ability->BeginComboAttack();
	}
	else if (GetCharacterMovement()->IsFalling() && Stat->GetCurrentEnergy() >= Ability->JumpAttackUseEnergy)
	{
		Ability->BeginJumpAttack();
	}
}

void AThroneCharacter::BeginDefend()
{
	Ability->BeginShieldUp();
}

void AThroneCharacter::EndDefend()
{
	Ability->EndShieldUp();
}

void AThroneCharacter::AcquisitionItem()
{
	if (bHasWeapon)
	{
		return;
	}

	if (GetPlayerController()->IsDisplay() && !AimKnifeDatas[CurrentAimKnife]->bCanThrow)
	{
		Sword->SetSkeletalMesh(ItemData->SwordMesh);
		Shield->SetSkeletalMesh(ItemData->ShieldMesh);

		ChangeCharacterControl();

		UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
		AnimInstance->bIsDefault = false;
		bHasWeapon = true;
	}
}

void AThroneCharacter::GateLeverInteract()
{
	if (GateLever == nullptr)
	{
		return;
	}
	if (!GateLever->GetGateLeverFlag())
	{
		return;
	}

	GateLever->OnTriggerGateLever();

	float X = GateLever->GetInteractLocation().X;
	float Y = GateLever->GetInteractLocation().Y;
	SetActorLocation(FVector(X, Y, GetActorLocation().Z));
	SetActorRotation(GateLever->GetInteractRotation());

	if (GateLever->GetLeverType() == ELeverType::Open)
	{
		Ability->BeginLeverOpen();
	}
	else
	{
		Ability->BeginLeverClose();
	}
}

void AThroneCharacter::DoorInteract()
{
	if (Door == nullptr)
	{
		return;
	}

	Door->OnTriggerDoor();
}

void AThroneCharacter::DragonGateInteract()
{
	if (DragonGate == nullptr)
	{
		return;
	}

	DragonGate->OnDragonGateTrigger();
	Ability->BeginDragonGateOpen(DragonGate->GetGateType());

	float X = DragonGate->GetInteractLocation().X;
	float Y = DragonGate->GetInteractLocation().Y;

	SetActorLocation(FVector(X, Y, GetActorLocation().Z));
	SetActorRotation(DragonGate->GetInteractRotation());
}

void AThroneCharacter::LadderInteract()
{
	if (Ladder == nullptr)
	{
		return;
	}

	Ladder->OnLadderClimb();

	UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		SetCharacterControl(ECharacterMode::Ladder);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		GetCharacterMovement()->MaxFlySpeed = 100.0f;
		GetCharacterMovement()->BrakingDecelerationFlying = 2048.0f;
		AnimInstance->bCanClimbingLadder = ~AnimInstance->bCanClimbingLadder;
	}
}

void AThroneCharacter::Sheath()
{
	if (CurrentCharacterMode == ECharacterMode::Default && bHasWeapon)
	{
		Ability->BeginOutSheath();
	}
	else if (CurrentCharacterMode == ECharacterMode::HoldWeapon)
	{
		Ability->BeginSheath();
	}
}

void AThroneCharacter::AttachWeaponSheath()
{
	Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Sheath_l"));
	Shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("SwordonBack"));

	ChangeCharacterControl();
}

void AThroneCharacter::AttachWeaponHand()
{
	Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("weapon_r"));
	Shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("shield_l"));

	ChangeCharacterControl();
}

void AThroneCharacter::BeginAimKnife(const FInputActionValue& Value)
{
	bIsAiming = Value.Get<bool>();

	Ability->BeginAimKnife();
	CurrentAimKnife = 0;
	SetAimKnifeData(CurrentAimKnife);

	Knife->SetSkeletalMesh(ItemData->KnifeMesh);
	Knife->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("weapon_r"));
}

void AThroneCharacter::EndAimKnife()
{
	bIsAiming = false;

	Ability->EndAimKnife();
	CurrentAimKnife = 1;
	SetAimKnifeData(CurrentAimKnife);

	Knife->SetSkeletalMesh(nullptr);
	Knife->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void AThroneCharacter::FireKnife()
{
	if (bIsAiming)
	{
		FVector SpawnLocation = GetMesh()->GetSocketLocation(TEXT("weapon_r"));
		FRotator SpawnRotation = GetMesh()->GetSocketRotation(TEXT("weapon_r"));
	
		AKnife* KnifeActor = GetWorld()->SpawnActor<AKnife>(KnifeClass, SpawnLocation, SpawnRotation);
		
		KnifeActor->SetDirection(GetActorForwardVector());
		KnifeActor->SetOwner(this);
	}
}

void AThroneCharacter::Death()
{
	AThronePlayerController* PlayerController = GetPlayerController();	
	if (PlayerController)
	{
		DisableInput(PlayerController);
	}
	Ability->BeginDead();
}

AThronePlayerController* AThroneCharacter::GetPlayerController()
{
	AThronePlayerController* PlayerController = CastChecked<AThronePlayerController>(GetController());
	return PlayerController;
}

void AThroneCharacter::SetAimKnifeData(const uint8 Index)
{
	if (!AimKnifeDatas.IsValidIndex(Index))
	{
		return;
	}

	SpringArm->TargetArmLength = AimKnifeDatas[Index]->TargetArmLength;
	SpringArm->bUsePawnControlRotation = AimKnifeDatas[Index]->UsePawnControlRotation;
	GetCharacterMovement()->bOrientRotationToMovement = AimKnifeDatas[Index]->OrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = AimKnifeDatas[Index]->UseControllerDesiredRotation;
	bUseControllerRotationYaw = AimKnifeDatas[Index]->UseControllerRotationYaw;

	if (Index == 0)
	{
		SpringArm->SetRelativeLocation(FVector(0.0f, 60.0f, 70.0f));
		GetCharacterMovement()->MaxWalkSpeed = Stat->GetWalkMoveSpeed();
	}
	else if (Index == 1)
	{
		SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
		GetCharacterMovement()->MaxWalkSpeed = Stat->GetRunMoveSpeed();
	}
}



