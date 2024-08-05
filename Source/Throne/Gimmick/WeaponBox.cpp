// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/WeaponBox.h"
#include "Components/BoxComponent.h"
#include "Item/ItemData.h"
#include "Interface/ItemAcquisitionInterface.h"

AWeaponBox::AWeaponBox()
{
	PrimaryActorTick.bCanEverTick = true;

	Sword = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Sword"));
	Sword->SetupAttachment(RootComponent);

	Shield = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shield"));
	Shield->SetupAttachment(RootComponent);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	CollisionBox->SetupAttachment(Sword);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBox::OnItemGetAreaBeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AWeaponBox::OnItemGetAreaEndOverlap);
	CollisionBox->SetCollisionProfileName(TEXT("ThroneTrigger"));

	static ConstructorHelpers::FObjectFinder<UItemData> ItemDataRef(TEXT("/Script/Throne.ItemData'/Game/Throne/Item/DA_Item.DA_Item'"));
	if (ItemDataRef.Object)
	{
		ItemData = ItemDataRef.Object;
	}

	Sword->SetSkeletalMesh(ItemData->SwordMesh);
	Shield->SetSkeletalMesh(ItemData->ShieldMesh);

}

// Called when the game starts or when spawned
void AWeaponBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBox::OnItemGetAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IItemAcquisitionInterface* ItemInterface = Cast<IItemAcquisitionInterface>(OtherActor);
	if (ItemInterface)
	{
		ItemInterface->BeginOverlapTakeItem(ItemData);
	}
}

void AWeaponBox::OnItemGetAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IItemAcquisitionInterface* ItemInterface = Cast<IItemAcquisitionInterface>(OtherActor);
	if (ItemInterface)
	{
		UE_LOG(LogTemp, Display, TEXT("WeaponBox EndOverlap"));
		ItemInterface->EndOverlapTakeItem();
	}
}

