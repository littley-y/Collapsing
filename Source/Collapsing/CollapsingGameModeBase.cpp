// Copyright Epic Games, Inc. All Rights Reserved.

#include "CollapsingGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Tile/TileGenerator.h"

ACollapsingGameModeBase::ACollapsingGameModeBase()
{
	TileGenerator = CreateDefaultSubobject<UTileGenerator>(TEXT("TileGenerator"));
}

void ACollapsingGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	FString BasicMapString;
	FFileHelper::LoadFileToString(BasicMapString, *(FPaths::GameSourceDir() + "/Collapsing/Data/TextMaps/Test.txt"));
	check(!BasicMapString.IsEmpty())
	UE_LOG(LogTemp, Warning, TEXT("Map Loaded : %s"), *BasicMapString);
	TileGenerator->SetMapString(BasicMapString);

	TileGenerator->GenerateMaps();
}
