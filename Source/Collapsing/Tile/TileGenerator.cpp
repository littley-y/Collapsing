// Fill out your copyright notice in the Description page of Project Settings.

#include "TileGenerator.h"

void FTileGeneratorTransform::UpdateVectorXY(const float InValue)
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

	Rotator.Yaw = FMath::Fmod(Rotator.Yaw, 360.f);
	if (FMath::IsNearlyZero(Rotator.Yaw))
	{
		Rotator.Yaw = 0.f;
	}
}

void FTileGeneratorTransform::UpdateVectorZ(const float InValue)
{

}

UTileGenerator::UTileGenerator()
{
	LoadBPFloor(TEXT("/Game/Collapsing/Blueprints/BP_BasicFloor"));
	LoadBPFloor(TEXT("/Game/Collapsing/Blueprints/BP_LeftCornerFloor"));
	LoadBPFloor(TEXT("/Game/Collapsing/Blueprints/BP_RightCornerFloor"));
	LoadBPFloor(TEXT("/Game/Collapsing/Blueprints/BP_SpeedFloor"));

	GeneratedFloorArray.SetNum(MaxTileNum + 1);
	CurrentMapIndex = 0;
}

void UTileGenerator::LoadBPFloor(const FString& BPPath)
{
	ConstructorHelpers::FClassFinder<AActor> BPFloor(*BPPath);
	if (BPFloor.Succeeded())
	{
		const int32 Position = BPPath.Find(TEXT("/"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		BPFloorArray.Emplace(BPFloor.Class);
		UE_LOG(LogTemp, Warning, TEXT("%s Loaded"), *BPPath.RightChop(Position + 1));
	}
}

void UTileGenerator::SpawnFloorTile()
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
		TileGenTrans.UpdateVectorXY(TileSize);
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
		TileGenTrans.UpdateVectorXY(TileSize);
		TileGenTrans.Rotator.Yaw += 90.f;
		TileGenTrans.UpdateVectorXY(TileSize);
	}
	CurrentMapIndex++;

	UE_LOG(LogTemp, Warning, TEXT("Floor Gen Location : %s"), *TileGenTrans.Vector.ToString())
	UE_LOG(LogTemp, Warning, TEXT("Floor Gen Yaw : %f"), TileGenTrans.Rotator.Yaw)
}

void UTileGenerator::InitMaps()
{
	for (int32 It = 0; It != InitTileNum; ++It)
	{
		SpawnFloorTile();
	}
	UE_LOG(LogTemp, Warning, TEXT("Init Tiles Generated : %d"), MaxTileNum)
}