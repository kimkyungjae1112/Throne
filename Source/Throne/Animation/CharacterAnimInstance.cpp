// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UCharacterAnimInstance::UCharacterAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 30.0f;
	GroundSpeed = 0.0f;
	bIsIdle = false;
	bIsJumping = false;
	bIsFalling = false;
	bIsShield = false;
	bIsDefault = true;
	bIsAimKnife = false;
	bCanClimbingLadder = false;
	bCanClimbUp = false;
	bCanClimbDown = false;
	Velocity = FVector::Zero();
	Angle = FVector::Zero();
}

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(GetOwningActor());
	if (Character)
	{
		Movement = Character->GetCharacterMovement();
	}
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = UKismetMathLibrary::VSize(Velocity);
		Angle = UKismetMathLibrary::InverseTransformDirection(Character->GetActorTransform(), Velocity);
		bIsIdle = MovingThreshould > Velocity.Size2D();
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
	}

	if (bCanClimbingLadder)
	{
		if (Velocity.Z > 0.0f)
		{
			bCanClimbUp = true;
			bCanClimbDown = false;
		}
		else if (Velocity.Z < 0.0f)
		{
			bCanClimbUp = false;
			bCanClimbDown = true;
		}
		else
		{
			bCanClimbUp = false;
			bCanClimbDown = false;
		}
	}
	else
	{
		bCanClimbUp = false;
		bCanClimbDown = false;
	}
}
