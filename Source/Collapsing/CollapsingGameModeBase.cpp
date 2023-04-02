// Copyright Epic Games, Inc. All Rights Reserved.


#include "CollapsingGameModeBase.h"
#include "Kismet/GameplayStatics.h"


ACollapsingGameModeBase::ACollapsingGameModeBase()
{
	AddBPFloor(TEXT("/Game/Collapsing/Blueprints/BP_BasicFloor"));
	AddBPFloor(TEXT("/Game/Collapsing/Blueprints/BP_LeftCornerFloor"));
	AddBPFloor(TEXT("/Game/Collapsing/Blueprints/BP_RightCornerFloor"));
}

void ACollapsingGameModeBase::AddBPFloor(const FString BPPath)
{
	ConstructorHelpers::FClassFinder<AActor> BPFloor(*BPPath);
	if (BPFloor.Succeeded())
	{
		FloorArray.Emplace(BPFloor.Class);
		const int32 Position = BPPath.Find(TEXT("/"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		UE_LOG(LogTemp, Warning, TEXT("%s Loaded"), *BPPath.RightChop(Position + 1));
	}
}

void ACollapsingGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GenerateMaps();
}

void ACollapsingGameModeBase::GenerateMaps()
{
	FString BasicMap;
	FFileHelper::LoadFileToString(BasicMap, *(FPaths::ProjectDir() + "TextMaps/test.txt"));
	check(!BasicMap.IsEmpty())
	UE_LOG(LogTemp, Warning, TEXT("Map Loaded : %s"), *BasicMap);
	AddFloorTile(BasicMap);
}

void ACollapsingGameModeBase::AddFloorTile(FString MapName)
{
	FVector ConstructVec(0.f, 0.f, 0.f);
	FRotator ConstructRot(0.f, 0.f, 0.f);

	for (const auto& It : MapName)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Val : %c"), It)
		if (It == 'B')
		{
			GetWorld()->SpawnActor<AActor>(FloorArray[Basic], ConstructVec, ConstructRot);
			ConstructVec.X += 400.f;
		}
		else if (It == 'L')
		{
			GetWorld()->SpawnActor<AActor>(FloorArray[LeftCorner], ConstructVec, ConstructRot);
			ConstructVec.Y -= 400.f;
		}
		else if (It == 'R')
		{
			GetWorld()->SpawnActor<AActor>(FloorArray[RightCorner], ConstructVec, ConstructRot);
			ConstructVec.Y += 400.f;
		}
	}
}