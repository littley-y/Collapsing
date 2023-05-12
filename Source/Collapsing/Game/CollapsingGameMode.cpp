// Copyright Epic Games, Inc. All Rights Reserved.

#include "CollapsingGameMode.h"
#include "Tile/TileGenerator.h"
#include "Utils/AssetFinder.hpp"
#include "CollapsingGameInstance.h"

ACollapsingGameMode::ACollapsingGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	TileGenerator = CreateDefaultSubobject<UTileGenerator>(TEXT("TileGenerator"));

	static TSubclassOf<APawn> CollapsingPawn = MyFunction::AssetClassFinder<APawn>(
		TEXT("/Game/Collapsing/Character/BP_CCharacter.BP_CCharacter_C"));
	if (IsValid(CollapsingPawn))
	{
		DefaultPawnClass = CollapsingPawn;
	}

	static TSubclassOf<AController> CollapsingController = MyFunction::AssetClassFinder<AController>(
		TEXT("/Game/Collapsing/Input/BPC_CPlayerController.BPC_CPlayerController_C"));
	if (IsValid(CollapsingController))
	{
		PlayerControllerClass = CollapsingController;
	}

	TileGenerateTime = 0.5f;
}

void ACollapsingGameMode::SetMapBasicString() const
{
	FString TempMapString;
	const FString FileString(TEXT("/Collapsing/Data/TextMaps/BasicMap.txt"));
	FFileHelper::LoadFileToString(TempMapString, *(FPaths::Combine(FPaths::GameSourceDir(), FileString)));
	check(!TempMapString.IsEmpty());

	const FString MapString(TempMapString);
	TileGenerator->SetMapString(MapString);
	UE_LOG(LogTemp, Warning, TEXT("Map Loaded : %s"), *MapString);
}

void ACollapsingGameMode::SetTileGenerateTimer(const float TargetTime)
{
	TileGenerateTime = TargetTime;
}

void ACollapsingGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(TileGenerator))
	{
		SetMapBasicString();
		TileGenerator->InitMaps();
	}

	const UWorld* CurrentWorld = GetWorld();
	if (IsValid(CurrentWorld))
	{
		CurrentWorld->GetTimerManager().SetTimer(TileGenerateTimerHandle, this, &ACollapsingGameMode::GenerateTile,
		                                         TileGenerateTime, true);
		CurrentWorld->GetTimerManager().SetTimer(CollapsedTimerHandle, this, &ACollapsingGameMode::DecreaseCollapse,
		                                         1.f, true);
	}
}

void ACollapsingGameMode::GenerateTile() const
{
	if (IsValid(TileGenerator))
	{
		TileGenerator->SpawnFloorTile();
	}
}

void ACollapsingGameMode::DecreaseCollapse() const
{
	UCollapsingGameInstance* MyGameInstance = Cast<UCollapsingGameInstance>(GetGameInstance());
	if (IsValid(MyGameInstance))
	{
		const float CurrentCollapsed = MyGameInstance->GetCollapsed();
		MyGameInstance->SetCollapsed(CurrentCollapsed - 1.f);
	}
}
