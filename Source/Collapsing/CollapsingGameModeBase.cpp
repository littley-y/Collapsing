// Copyright Epic Games, Inc. All Rights Reserved.

#include "CollapsingGameModeBase.h"
#include "Tile/TileGenerator.h"

ACollapsingGameModeBase::ACollapsingGameModeBase()
{
	TileGenerator = CreateDefaultSubobject<UTileGenerator>(TEXT("TileGenerator"));
}

void ACollapsingGameModeBase::SetTileGenerator() const
{
	FString BasicMapString;
	const FString FileString(TEXT("/Collapsing/Data/TextMaps/BasicMap.txt"));

	FFileHelper::LoadFileToString(BasicMapString, *(FPaths::Combine(FPaths::GameSourceDir(), FileString)));
	check(!BasicMapString.IsEmpty())
	TileGenerator->SetMapString(BasicMapString);
	UE_LOG(LogTemp, Warning, TEXT("Map Loaded : %s"), *BasicMapString);
}

void ACollapsingGameModeBase::GenerateTile() const
{
	TileGenerator->AddFloorTile();
}

void ACollapsingGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SetTileGenerator();
	TileGenerator->InitMaps();
}
