// Copyright Epic Games, Inc. All Rights Reserved.

#include "CollapsingGameMode.h"
#include "CTileManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CMapGenerator.h"

ACollapsingGameMode::ACollapsingGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);

	TileSpawnTime = 1.6f;
	TileDestroyTime = 0.8f;
	DestroyDelay = 4.f;
	MaxTileNum = 20;
	StartPosition = {-1000.f, 0.f, 1000.f};
	bInitTileDestroyed = false;

	TileManager = CreateDefaultSubobject<UCTileManager>(TEXT("TileManager"));
	MapGenerator = CreateDefaultSubobject<UCMapGenerator>(TEXT("MapGenerator"));
	ArcadeMapString = MapGenerator->GenerateString(100);

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

	static ConstructorHelpers::FClassFinder<AActor> InitTileRef(
		TEXT("/Script/Engine.Blueprint'/Game/Collapsing/Tile/BP_CInitTile.BP_CInitTile_C'"));
	if (IsValid(InitTileRef.Class))
	{
		BP_InitTile = InitTileRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> GeometryInitTileRef(
		TEXT("/Script/Engine.Blueprint'/Game/Collapsing/Tile/Geometry/GA_CInitTile.GA_CInitTile_C'"));
	if (IsValid(GeometryInitTileRef.Class))
	{
		GeometryInitTile = GeometryInitTileRef.Class;
	}
}

void ACollapsingGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(BP_InitTile))
	{
		const FVector InitTileLocation = {-800.f, -200.f, 0.f};
		const FRotator InitTileRotation = {0.f, 90.f, 0.f};
		InitTile = GetWorld()->SpawnActor<AActor>(BP_InitTile, InitTileLocation, InitTileRotation);
	}
}

void ACollapsingGameMode::SetStageMapString()
{
	const FString MapPath = FPaths::Combine(FPaths::ProjectDir(), ("TextMaps/BasicMap.txt"));
	FFileHelper::LoadFileToString(StageMapString, *MapPath);

	if (FPaths::ValidatePath(StageMapString))
	{
		UE_LOG(LogTemp, Warning, TEXT("Map Paths : %s"), *MapPath);
		TileManager->SetMapString(StageMapString);
		UE_LOG(LogTemp, Warning, TEXT("Map Loaded : %s"), *StageMapString);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Path Invalid : %s"), *MapPath);
	}
}

void ACollapsingGameMode::SetArcadeMapString() const
{
	TileManager->SetMapString(ArcadeMapString);
	UE_LOG(LogTemp, Warning, TEXT("Map Loaded : %s"), *ArcadeMapString);
}

void ACollapsingGameMode::SetTimer(int32 InTimerType)
{
	if (InTimerType == 0)
	{
		GetWorldTimerManager().ClearTimer(SpawnTileTimerHandle);
		GetWorldTimerManager().SetTimer(SpawnTileTimerHandle, this, &ACollapsingGameMode::SetTileGenerate,
		                                TileDestroyTime, true);
	}
	else if (InTimerType == 1)
	{
		GetWorldTimerManager().ClearAllTimersForObject(this);
	}
}

void ACollapsingGameMode::StartGame(const FString MapType)
{
	if (IsValid(TileManager) && IsValid(MapGenerator))
	{
		if (MapType == "Stage")
		{
			SetStageMapString();
		}
		else if (MapType == "Arcade")
		{
			SetArcadeMapString();
		}

		TileManager->InitMaps(StartPosition, MaxTileNum);

		const UWorld* CurrentWorld = GetWorld();
		if (IsValid(CurrentWorld))
		{
			TileDestroyTime = FMath::Clamp(TileDestroyTime, 0.5f, 10.f);
			TileSpawnTime = FMath::Clamp(TileSpawnTime, 0.5f, TileDestroyTime);

			CurrentWorld->GetTimerManager().SetTimer(SpawnTileTimerHandle, this, &ACollapsingGameMode::SetTileGenerate,
			                                         TileSpawnTime, true);
			CurrentWorld->GetTimerManager().SetTimer(DestroyTileTimerHandle, this, &ACollapsingGameMode::SetTileDestroy,
			                                         TileDestroyTime, true, DestroyDelay);
		}
	}
}

void ACollapsingGameMode::ExitGame()
{
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("EXIT"));
}

void ACollapsingGameMode::RestartGame()
{
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("RestartLevel"));
}

void ACollapsingGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
}

void ACollapsingGameMode::SetTileGenerate() const
{
	if (IsValid(TileManager))
	{
		TileManager->SpawnTile();
	}
}

void ACollapsingGameMode::SetTileDestroy()
{
	if (bInitTileDestroyed == false)
	{
		InitTile->SetActorHiddenInGame(true);
		InitTile->SetLifeSpan(0.1f);
		AActor* DestroyedInitTile = GetWorld()->SpawnActor<AActor>(GeometryInitTile, {-800.f, -200.f, 0.f},
		                                                           {0.f, 90.f, 0.f});
		DestroyedInitTile->SetLifeSpan(1.f);
		bInitTileDestroyed = true;
	}

	if (IsValid(TileManager))
	{
		TileManager->DestroyTile();
	}
}
