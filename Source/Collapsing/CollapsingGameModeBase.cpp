// Copyright Epic Games, Inc. All Rights Reserved.

#include "CollapsingGameModeBase.h"
#include "Tile/TileGenerator.h"

ACollapsingGameModeBase::ACollapsingGameModeBase()
{
	TileGenerator = CreateDefaultSubobject<UTileGenerator>(TEXT("TileGenerator"));
}

void ACollapsingGameModeBase::SetMapBasicString() const
{
	FString MapString;
	const FString FileString(TEXT("/Collapsing/Data/TextMaps/BasicMap.txt"));
	FFileHelper::LoadFileToString(MapString, *(FPaths::Combine(FPaths::GameSourceDir(), FileString)));
	check(!MapString.IsEmpty())

	TileGenerator->SetMapString(MapString);
	UE_LOG(LogTemp, Warning, TEXT("Map Loaded : %s"), *MapString);
}

void ACollapsingGameModeBase::GenerateTile() const
{
	TileGenerator->SpawnFloorTile();
}

void ACollapsingGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SetMapBasicString();
	TileGenerator->InitMaps();
}
