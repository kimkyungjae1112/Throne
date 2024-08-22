// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/Ladder.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Blueprint/UserWidget.h"
#include "GimmickInterface/LadderInterface.h"

ALadder::ALadder()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	RootComponent = SplineComp;

	TopBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Top Box"));
	TopBox->SetupAttachment(RootComponent);
	TopBox->SetCollisionProfileName(TEXT("ThroneTrigger"));
	TopBox->OnComponentBeginOverlap.AddDynamic(this, &ALadder::OnLadderTopBeginOverlap);
	TopBox->OnComponentEndOverlap.AddDynamic(this, &ALadder::OnLadderTopEndOverlap);

	BottomBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Bottom Box"));
	BottomBox->SetupAttachment(RootComponent);
	BottomBox->SetCollisionProfileName(TEXT("ThroneTrigger"));
	BottomBox->OnComponentBeginOverlap.AddDynamic(this, &ALadder::OnLadderBottomBeginOverlap);
	BottomBox->OnComponentEndOverlap.AddDynamic(this, &ALadder::OnLadderBottomEndOverlap);

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassRef(TEXT("/Game/Throne/UI/WBP_Interact.WBP_Interact_C"));
	if (WidgetClassRef.Class)
	{
		WidgetClass = WidgetClassRef.Class;
	}


	bIsPlayer = false;
}

void ALadder::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALadder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALadder::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ClearLadderMesh();

	// 스플라인 길이 얻기
	float SplineLength = SplineComp->GetSplineLength();

	// 사다리 발판 배치
	float StepInterval = 50.0f; // 발판 간격
	int32 NumSteps = FMath::CeilToInt(SplineLength / StepInterval);

	for (int32 i = 0; i < NumSteps; ++i)
	{
		// 스플라인에서 위치 및 회전 얻기
		FVector Location = SplineComp->GetLocationAtDistanceAlongSpline(i * StepInterval, ESplineCoordinateSpace::World);
		FRotator Rotation = SplineComp->GetRotationAtDistanceAlongSpline(i * StepInterval, ESplineCoordinateSpace::World);

		// 사다리 발판 메쉬 컴포넌트 생성
		UStaticMeshComponent* NewStep = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
		NewStep->SetStaticMesh(LadderStep); // 발판 메쉬 설정
		NewStep->SetWorldLocation(Location);
		NewStep->SetWorldRotation(Rotation);
		NewStep->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
		NewStep->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

		// 메쉬 활성화
		NewStep->RegisterComponent();
		LadderStepComponents.Add(NewStep);
	}

	// 사다리 프레임 배치
	float FrameSpacing = 150.0f;

	for (float Distance = 0; Distance < SplineLength; Distance += FrameSpacing)
	{
		// 각 프레임의 위치를 Spline에서 가져오기.
		FVector FrameLocation = SplineComp->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

		// 프레임의 회전을 Spline의 방향에 맞추어 설정한다.
		FRotator FrameRotation = SplineComp->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

		// 사다리 프레임을 생성하고 위치와 회전을 설정한다.
		UStaticMeshComponent* NewFrame = NewObject<UStaticMeshComponent>(this);
		NewFrame->SetCollisionProfileName(TEXT("NoCollision"));
		NewFrame->SetStaticMesh(LadderFrame); // 사다리 프레임 메쉬 설정
		NewFrame->SetWorldLocation(FrameLocation);
		FRotator AdjustedRotation = FrameRotation + FRotator(0.0f, 180.0f, 0.0f); // 예시로 90도 회전
		NewFrame->SetWorldRotation(AdjustedRotation);

		// 생성된 프레임을 RootComponent에 부착한다.
		NewFrame->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
		NewFrame->RegisterComponent();

		// 리스트에 추가하여 이후 정리할 때 사용한다.
		LadderFrameComponents.Add(NewFrame);
	}
}

void ALadder::ClearLadderMesh()
{
	for (UStaticMeshComponent* Comp : LadderStepComponents)
	{
		if (Comp)
		{
			Comp->DestroyComponent();
		}
	}
	LadderStepComponents.Empty();

	for (UStaticMeshComponent* Comp : LadderFrameComponents)
	{
		if (Comp)
		{
			Comp->DestroyComponent();
		}
	}
	LadderFrameComponents.Empty();
}

void ALadder::OnLadderTopBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	WidgetPtr = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	if (WidgetPtr && !bIsPlayer)
	{
		WidgetPtr->AddToViewport();
	}

	ILadderInterface* Interface = Cast<ILadderInterface>(OtherActor);
	if (Interface)
	{
		Interface->SetLadder(this);
	}

	if (bIsPlayer)
	{
		UE_LOG(LogTemp, Display, TEXT("Top End Broadcast"));
		OnTopEndClimb.ExecuteIfBound();
		bIsPlayer = false;
	}
}

void ALadder::OnLadderTopEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (WidgetPtr->IsInViewport())
	{
		WidgetPtr->RemoveFromViewport();
	}

	ILadderInterface* Interface = Cast<ILadderInterface>(OtherActor);
	if (Interface)
	{
		Interface->SetLadder(nullptr);
	}
}

void ALadder::OnLadderBottomBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	WidgetPtr = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	if (WidgetPtr && !bIsPlayer)
	{
		WidgetPtr->AddToViewport();
	}

	ILadderInterface* Interface = Cast<ILadderInterface>(OtherActor);
	if (Interface)
	{
		Interface->SetLadder(this);
	}

	if (bIsPlayer)
	{
		UE_LOG(LogTemp, Display, TEXT("Bottom End BroadCast"));
		OnBottomEndClimb.ExecuteIfBound();
		bIsPlayer = false;
	}
}

void ALadder::OnLadderBottomEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (WidgetPtr->IsInViewport())
	{
		WidgetPtr->RemoveFromViewport();
	}

	ILadderInterface* Interface = Cast<ILadderInterface>(OtherActor);
	if (Interface)
	{
		Interface->SetLadder(nullptr);
	}
}

void ALadder::OnLadderClimb()
{
	bIsPlayer = true;
}

