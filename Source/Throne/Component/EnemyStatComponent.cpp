// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EnemyStatComponent.h"
#include "EnemyData/EnemyStatData.h"
UEnemyStatComponent::UEnemyStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UEnemyStatData> StatDataRef(TEXT("/Script/Throne.EnemyStatData'/Game/Throne/Enemy/DA_EnemyStat.DA_EnemyStat'"));
	if (StatDataRef.Object)
	{
		StatData = StatDataRef.Object;
	}

	AttackDamage = StatData->AttackDamage;
	AttackRange = StatData->AttackRange;
	AttackSpeed = StatData->AttackSpeed;
	AttackRadian = StatData->AttackRadian;
	MoveSpeed = StatData->MoveSpeed;
	MaxHp = StatData->Hp;
	CurrentHp = MaxHp;
}

void UEnemyStatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UEnemyStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

float UEnemyStatComponent::ApplyDamage(float InDamage)
{
	float ActualDamage = FMath::Clamp(InDamage, 0, InDamage);
	float PrevHp = CurrentHp - InDamage;

	SetHp(PrevHp);
	if (PrevHp <= 0)
	{
		OnHpZero.Broadcast();
	}
	return ActualDamage;
}

void UEnemyStatComponent::SetHp(float NewHp)
{
	CurrentHp = NewHp;
	UE_LOG(LogTemp, Display, TEXT("Enemy CurrentHp : %f"), CurrentHp);
	OnHpChanged.Broadcast(CurrentHp);
}

