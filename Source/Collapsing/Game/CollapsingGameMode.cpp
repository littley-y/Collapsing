// Copyright Epic Games, Inc. All Rights Reserved.

#include "CollapsingGameMode.h"
#include "Tile/TileGenerator.h"
#include "Utils/AssetFinder.hpp"

ACollapsingGameMode::ACollapsingGameMode()
{
	TileGenerator = CreateDefaultSubobject<UTileGenerator>(TEXT("TileGenerator"));

	static TSubclassOf<APawn> CollapsingPawn = MyFunction::AssetClassFinder<APawn>(TEXT("/Game/Collapsing/Blueprints/BP_CCharacter.BP_CCharacter_C"));
	if (IsValid(CollapsingPawn))
	{
		DefaultPawnClass = CollapsingPawn;
	}

	static TSubclassOf<AController> CollapsingController = MyFunction::AssetClassFinder<AController>(TEXT("/Game/Collapsing/Input/BPC_CPlayerController.BPC_CPlayerController_C"));
	if (IsValid(CollapsingController))
	{
		PlayerControllerClass = CollapsingController;
	}
}

void ACollapsingGameMode::SetMapBasicString() const
{
	FString TempMapString;
	const FString FileString(TEXT("/Collapsing/Data/TextMaps/BasicMap.txt"));
	FFileHelper::LoadFileToString(TempMapString, *(FPaths::Combine(FPaths::GameSourceDir(), FileString)));
	check(!TempMapString.IsEmpty())

	const FString MapString(TempMapString);
	TileGenerator->SetMapString(MapString);
	UE_LOG(LogTemp, Warning, TEXT("Map Loaded : %s"), *MapString);
}

void ACollapsingGameMode::GenerateTile() const
{
	if (IsValid(TileGenerator))
	{
		TileGenerator->SpawnFloorTile();
	}
}

void ACollapsingGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(TileGenerator))
	{
		SetMapBasicString();
		TileGenerator->InitMaps();
	}
}
