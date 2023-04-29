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

	if (FMath::IsNearlyEqual(FMath::Abs(Rotator.Yaw), 360.f))
	{
		Rotator.Yaw = 0.f;
	}
}

void FTileGeneratorTransform::UpdateVectorZ(const float InValue)
{

}

UTileGenerator::UTileGenerator()
{
	LoadBPFloor(TEXT("/Game/Collapsing/Blueprints/BP_CBasicFloor"), '0');
	LoadBPFloor(TEXT("/Game/Collapsing/Blueprints/BP_CLeftCornerFloor"), 'L');
	LoadBPFloor(TEXT("/Game/Collapsing/Blueprints/BP_CRightCornerFloor"), 'R');
	LoadBPFloor(TEXT("/Game/Collapsing/Blueprints/BP_CSpeedFloor"), 'S');
	LoadBPFloor(TEXT("/Game/Collapsing/Blueprints/BP_CBrokenFloor"), 'B');

	GeneratedFloorArray.SetNum(MaxTileNum + 1);
	CurrentMapIndex = 0;
}

void UTileGenerator::LoadBPFloor(const FString& BPPath, uint8 KeyChar)
{
	ConstructorHelpers::FClassFinder<AActor> BPFloor(*BPPath);
	if (BPFloor.Succeeded())
	{
		const int32 Position = BPPath.Find(TEXT("/"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		BPFloorArray.Emplace(KeyChar, BPFloor.Class);
		UE_LOG(LogTemp, Warning, TEXT("%s Loaded"), *BPPath.RightChop(Position + 1));
	}
}

void UTileGenerator::SpawnFloorTile()
{
	const TCHAR& MapChar = MapString[CurrentMapIndex % MapString.Len()];
	const int32 ArrayIndex = CurrentMapIndex % MaxTileNum;
	UE_LOG(LogTemp, Warning, TEXT("Current Val : %c"), MapChar)

	if (CurrentMapIndex >= MaxTileNum && IsValid(GeneratedFloorArray[ArrayIndex]))
	{
		GeneratedFloorArray[ArrayIndex]->Destroy();
	}

	GeneratedFloorArray[ArrayIndex] = GetWorld()->SpawnActor<AActor>(BPFloorArray[MapChar], TileGenTrans.Vector,
	                                                                 TileGenTrans.Rotator);
	if (MapChar == 'L')
	{
		TileGenTrans.Rotator.Yaw -= 90.f;
	}
	else if (MapChar == 'R')
	{
		TileGenTrans.UpdateVectorXY(TileSize);
		TileGenTrans.Rotator.Yaw += 90.f;
		TileGenTrans.UpdateVectorXY(TileSize);
	}
	else
	{
		TileGenTrans.UpdateVectorXY(TileSize);
	}

	CurrentMapIndex++;

	UE_LOG(LogTemp, Warning, TEXT("Floor Gen Location : %s, Yaw : %f"), *TileGenTrans.Vector.ToString(), TileGenTrans.Rotator.Yaw)
}

void UTileGenerator::InitMaps()
{
	for (int32 It = 0; It != InitTileNum; ++It)
	{
		SpawnFloorTile();
	}
	UE_LOG(LogTemp, Warning, TEXT("Init Tiles Generated : %d"), MaxTileNum)
}
