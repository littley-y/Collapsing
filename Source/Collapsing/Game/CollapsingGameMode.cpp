// Copyright Epic Games, Inc. All Rights Reserved.

#include "CollapsingGameMode.h"
#include "Tile/TileManager.h"

ACollapsingGameMode::ACollapsingGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	TileGenerator = CreateDefaultSubobject<UTileManager>(TEXT("TileGenerator"));

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

	SetTileGenerateTimer(.8f);
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
		CurrentWorld->GetTimerManager().SetTimer(TileGenerateTimerHandle, this, &ACollapsingGameMode::CallTileManager,
			TileGenerateTime, true, 4.f);
	}
}

void ACollapsingGameMode::CallTileManager() const
{
	if (IsValid(TileGenerator))
	{
		TileGenerator->ManageTile();
	}
}
