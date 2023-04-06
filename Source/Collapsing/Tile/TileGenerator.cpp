// Fill out your copyright notice in the Description page of Project Settings.

#include "TileGenerator.h"

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

	if (FMath::IsNearlyEqual(FMath::Abs(Rotator.Yaw), 360.f, 0.01f))
	{
		Rotator.Yaw = 0.f;
	}
}

UTileGenerator::UTileGenerator()
{
	AddBPFloor(TEXT("/Game/Collapsing/Blueprints/BP_BasicFloor"));
	AddBPFloor(TEXT("/Game/Collapsing/Blueprints/BP_LeftCornerFloor"));
	AddBPFloor(TEXT("/Game/Collapsing/Blueprints/BP_RightCornerFloor"));
	AddBPFloor(TEXT("/Game/Collapsing/Blueprints/BP_SpeedFloor"));

	GeneratedFloorArray.SetNum(MaxTileNum + 1);
}

void UTileGenerator::AddBPFloor(const FString& BPPath)
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

void UTileGenerator::AddFloorTile()
{
	const TCHAR& MapChar = MapString[CurrentMapIndex % MapString.Len()];
	const int32 ArrayIndex = CurrentMapIndex % MaxTileNum;
	UE_LOG(LogTemp, Warning, TEXT("Current Val : %c"), MapChar)

	if (CurrentMapIndex >= MaxTileNum)
	{
		GeneratedFloorArray[ArrayIndex]->Destroy();
	}

	if (MapChar == 'B' || MapChar == 'S')
	{
		GeneratedFloorArray[ArrayIndex] = GetWorld()->SpawnActor<AActor>(
			BPFloorArray[MapChar == 'B' ? Basic : Speed], TileGenTrans.Vector, TileGenTrans.Rotator);
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
}

void UTileGenerator::GenerateMaps()
{
	for (int32 It = 0; It != 5; ++It)
	{
		AddFloorTile();
	}
	UE_LOG(LogTemp, Warning, TEXT("Init Tiles Generated : %d"), MaxTileNum)
}