// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ThroneGameModeBase.h"
#include "ThroneGameModeBase.h"

AThroneGameModeBase::AThroneGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/Throne/Blueprints/BP_ThroneCharacter.BP_ThroneCharacter_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/Throne.ThronePlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
