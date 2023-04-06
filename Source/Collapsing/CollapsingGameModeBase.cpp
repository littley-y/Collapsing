// Copyright Epic Games, Inc. All Rights Reserved.

#include "CollapsingGameModeBase.h"
#include "Tile/TileGenerator.h"

ACollapsingGameModeBase::ACollapsingGameModeBase()
{
	TileGenerator = CreateDefaultSubobject<UTileGenerator>(TEXT("TileGenerator"));
}

void ACollapsingGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	FString BasicMapString;
	const FString FileString(TEXT("/Collapsing/Data/TextMaps/BasicMap.txt"));
	FFileHelper::LoadFileToString(BasicMapString, *(FPaths::Combine(FPaths::GameSourceDir(), FileString)));
	check(!BasicMapString.IsEmpty())
	UE_LOG(LogTemp, Warning, TEXT("Map Loaded : %s"), *BasicMapString);
	TileGenerator->SetMapString(BasicMapString);

	TileGenerator->GenerateMaps();
}
