// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/AbilityComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

UAbilityComponent::UAbilityComponent()
{

}


// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UAbilityComponent::BeginDefaultAttack()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();
	if (Owner && AnimInstance)
	{
		Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		UE_LOG(LogTemp, Display, TEXT("몽타주 실행"));
		AnimInstance->Montage_Play(DefaultAttackMontage);

		FOnMontageEnded MontageEnded;
		MontageEnded.BindUObject(this, &UAbilityComponent::EndDefaultAttack);
		AnimInstance->Montage_SetEndDelegate(MontageEnded, DefaultAttackMontage);
	}
}

void UAbilityComponent::EndDefaultAttack(UAnimMontage* Target, bool IsProperlyEnded)
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();
	if (Owner && AnimInstance)
	{
		Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}



