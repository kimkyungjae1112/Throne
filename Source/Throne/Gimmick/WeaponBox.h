// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBox.generated.h"

UCLASS()
class THRONE_API AWeaponBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void OnItemGetAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, Category = "Box")
	TObjectPtr<class UBoxComponent> CollisionBox;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> Sword;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> Shield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UItemData> ItemData;

};
