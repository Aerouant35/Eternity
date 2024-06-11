// Copyright Epic Games, Inc. All Rights Reserved.

#include "EternityGameMode.h"
#include "EternityCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEternityGameMode::AEternityGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
