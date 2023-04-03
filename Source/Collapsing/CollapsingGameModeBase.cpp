// Copyright Epic Games, Inc. All Rights Reserved.


#include "CollapsingGameModeBase.h"
#include "Kismet/GameplayStatics.h"


void FTileGeneratorTransform::MoveVector(const float InValue)
{
	const float CurrentDegrees = FMath::FindDeltaAngleDegrees(Rotator.Yaw, 0.f);
	if (FMath::IsNearlyEqual(CurrentDegrees, 0.f))
	{
		Vector.X += InValue;
	}
	else if (FMath::IsNearlyEqual(CurrentDegrees, -90.f))
	{
		Vector.Y += InValue;
	}
	else if (FMath::IsNearlyEqual(FMath::Abs(CurrentDegrees), 180.f))
	{
		Vector.X -= InValue;
	}
	else if (FMath::IsNearlyEqual(CurrentDegrees, 90.f))
	{
		Vector.Y -= InValue;
	}
}

ACollapsingGameModeBase::ACollapsingGameModeBase()
{
	AddBPFloor(TEXT("/Game/Collapsing/Blueprints/BP_BasicFloor"));
	AddBPFloor(TEXT("/Game/Collapsing/Blueprints/BP_LeftCornerFloor"));
	AddBPFloor(TEXT("/Game/Collapsing/Blueprints/BP_RightCornerFloor"));

	BasicMapString = "";
	CurrentMapIndex = 0;
	GeneratedFloorArray.SetNum(MaxTileNum + 1);
}

void ACollapsingGameModeBase::AddBPFloor(const FString BPPath)
{
	ConstructorHelpers::FClassFinder<AActor> BPFloor(*BPPath);
	const int32 Position = BPPath.Find(TEXT("/"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	if (BPFloor.Succeeded())	
	{
		BPFloorArray.Emplace(BPFloor.Class);
		UE_LOG(LogTemp, Warning, TEXT("%s Loaded"), *BPPath.RightChop(Position + 1));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Load Failed"), *BPPath.RightChop(Position + 1));
	}
}

void ACollapsingGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GenerateMaps();
}

void ACollapsingGameModeBase::GenerateMaps()
{
	FFileHelper::LoadFileToString(BasicMapString, *(FPaths::ProjectDir() + "TextMaps/test.txt"));
	check(!BasicMapString.IsEmpty())
	UE_LOG(LogTemp, Warning, TEXT("Map Loaded : %s"), *BasicMapString);
	for (int32 It = 0; It != 5; ++It)
	{
		AddFloorTile();
	}
}

void ACollapsingGameModeBase::AddFloorTile()
{
	const TCHAR& MapChar = BasicMapString[CurrentMapIndex];
	const int32 ArrayIndex = CurrentMapIndex % MaxTileNum;
	UE_LOG(LogTemp, Warning, TEXT("Current Val : %c"), MapChar)

	if (CurrentMapIndex >= MaxTileNum)
	{
		GeneratedFloorArray[ArrayIndex]->Destroy();
	}

	if (MapChar == 'B')
	{
		GeneratedFloorArray[ArrayIndex] = GetWorld()->SpawnActor<AActor>(
			BPFloorArray[Basic], TileGenTrans.Vector, TileGenTrans.Rotator);
		TileGenTrans.MoveVector(400.f);
	}
	else if (MapChar == 'L')
	{
		GeneratedFloorArray[ArrayIndex] = GetWorld()->SpawnActor<AActor>(
			BPFloorArray[LeftCorner], TileGenTrans.Vector, TileGenTrans.Rotator);
		TileGenTrans.Rotator.Yaw -= 90.f;
	}
	else if (MapChar == 'R')
	{
		GeneratedFloorArray[ArrayIndex] = GetWorld()->SpawnActor<AActor>(
			BPFloorArray[RightCorner], TileGenTrans.Vector, TileGenTrans.Rotator);
		TileGenTrans.MoveVector(400.f);
		TileGenTrans.Rotator.Yaw += 90.f;
		TileGenTrans.MoveVector(400.f);
	}
	UE_LOG(LogTemp, Warning, TEXT("Floor Gen Location : %s"), *TileGenTrans.Vector.ToString())
	UE_LOG(LogTemp, Warning, TEXT("Floor Gen Yaw : %f"), TileGenTrans.Rotator.Yaw)

	CurrentMapIndex++;
	if (CurrentMapIndex == BasicMapString.Len())
	{
		CurrentMapIndex = 0;
	}
}