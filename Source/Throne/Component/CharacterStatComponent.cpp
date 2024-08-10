// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CharacterStatComponent.h"
#include "Character/CharacterStatData.h"

UCharacterStatComponent::UCharacterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UCharacterStatData> StatDataRef(TEXT("/Script/Throne.CharacterStatData'/Game/Throne/Character/Data/DA_CharacterStat.DA_CharacterStat'"));
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

	EnergyRegenTime = 0.0f;
	HealthRegenTime = 0.0f;
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

	EnergyRegenTime += DeltaTime;
	HealthRegenTime += DeltaTime;

	if (EnergyRegenTime >= 1.0f)
	{
		if (CurrentEnergy < MaxEnergy)
		{
			SetEnergy(-5.0f);
		}
		EnergyRegenTime = 0.0f;
	}

	if (HealthRegenTime >= 3.0f)
	{
		if (CurrentHp <= MaxHp)
		{
			SetHp(CurrentHp + 1000.0f);
		}
		HealthRegenTime = 0.0f;
	}
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
	UE_LOG(LogTemp, Display, TEXT("CurrentEnergy : %f"), CurrentEnergy);
	OnEnergyChanged.Broadcast(CurrentEnergy);
}
