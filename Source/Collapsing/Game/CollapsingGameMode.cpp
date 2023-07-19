// Copyright Epic Games, Inc. All Rights Reserved.

#include "CollapsingGameMode.h"
#include "Tile/TileManager.h"

ACollapsingGameMode::ACollapsingGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	TileManager = CreateDefaultSubobject<UTileManager>(TEXT("TileManager"));

	static ConstructorHelpers::FClassFinder<APawn> CollapsingPawnRef(
		TEXT("/Game/Collapsing/Character/BP_CCharacter.BP_CCharacter_C"));
	if (IsValid(CollapsingPawnRef.Class))
	{
		DefaultPawnClass = CollapsingPawnRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AController> CollapsingControllerRef(
		TEXT("/Game/Collapsing/Input/BPC_CPlayerController.BPC_CPlayerController_C"));
	if (IsValid(CollapsingControllerRef.Class))
	{
		PlayerControllerClass = CollapsingControllerRef.Class;
	}

	TileSpawnTime = 1.6f;
	TileDestroyTime = 0.8f;
	DestroyDelay = 4.f;
	MaxTileNum = 20;
	StartPosition = { -1000.f, 0.f, 1000.f };
}

void ACollapsingGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(TileManager))
	{
		SetMapBasicString();
		TileManager->InitMaps(StartPosition, MaxTileNum);
	}

	const UWorld* CurrentWorld = GetWorld();
	if (IsValid(CurrentWorld))
	{
		TileDestroyTime = FMath::Clamp(TileDestroyTime, 0.5f, 2.f);
		TileSpawnTime = FMath::Clamp(TileSpawnTime, 0.5f, TileDestroyTime);

		CurrentWorld->GetTimerManager().SetTimer(SpawnTileTimerHandle, this, &ACollapsingGameMode::SetTileGenerate,
			TileSpawnTime, true);
		CurrentWorld->GetTimerManager().SetTimer(DestroyTileTimerHandle, this, &ACollapsingGameMode::SetTileDestroy,
			TileDestroyTime, true, DestroyDelay);
	}
}

void ACollapsingGameMode::SetMapBasicString()
{
	const FString FileString(TEXT("/Collapsing/Data/TextMaps/BasicMap.txt"));
	FFileHelper::LoadFileToString(MapString, *(FPaths::Combine(FPaths::GameSourceDir(), FileString)));
	check(!MapString.IsEmpty());

	TileManager->SetMapString(MapString);
	UE_LOG(LogTemp, Warning, TEXT("Map Loaded : %s"), *MapString);
}

void ACollapsingGameMode::SyncTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("Sync Timer"));
	GetWorldTimerManager().ClearTimer(SpawnTileTimerHandle);
	GetWorldTimerManager().SetTimer(SpawnTileTimerHandle, this, &ACollapsingGameMode::SetTileGenerate, TileDestroyTime, true);
}

void ACollapsingGameMode::SetTileGenerate() const
{
	if (IsValid(TileManager))
	{
		TileManager->SpawnTile();
	}
}

void ACollapsingGameMode::SetTileDestroy() const
{
	if (IsValid(TileManager))
	{
		TileManager->DestroyTile();
	}
}
