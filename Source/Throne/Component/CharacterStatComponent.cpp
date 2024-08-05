// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CharacterStatComponent.h"
#include "Character/CharacterStatData.h"

UCharacterStatComponent::UCharacterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UCharacterStatData> StatDataRef(TEXT("/Script/Throne.CharacterStatData'/Game/Throne/Character/DA_CharacterStat.DA_CharacterStat'"));
	if (StatDataRef.Object)
	{
		StatData = StatDataRef.Object;
	}

	AttackDamage = StatData->AttackDamage;
	AttackRange = StatData->AttackRange;
	AttackSpeed = StatData->AttackSpeed;
	AttackRadian = StatData->AttackRadian;
	WalkMoveSpeed = StatData->WalkMoveSpeed;
	RunMoveSpeed = StatData->RunMoveSpeed;
	MaxHp = StatData->Hp;
	MaxEnergy = StatData->Enegy;

	CurrentHp = MaxHp;
	CurrentEnergy = MaxEnergy;
}


// Called when the game starts
void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

float UCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
	return ActualDamage;
}

void UCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	UE_LOG(LogTemp, Display, TEXT("Player Current Hp : %f"), CurrentHp);
	OnHpChanged.Broadcast(CurrentHp);
}

void UCharacterStatComponent::SetEnergy(float UsedEnergy)
{
	CurrentEnergy -= UsedEnergy;
	OnEnergyChanged.Broadcast(CurrentEnergy);
}
