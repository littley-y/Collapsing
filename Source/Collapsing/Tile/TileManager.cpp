// Fill out your copyright notice in the Description page of Project Settings.

#include "TileManager.h"

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

UTileManager::UTileManager()
{
	LoadBPClass(BPFloorMap, "/Game/Collapsing/Tile/BP_CBasicFloor", '0');
	LoadBPClass(BPFloorMap, "/Game/Collapsing/Tile/BP_CLeftCornerFloor", 'L');
	LoadBPClass(BPFloorMap, "/Game/Collapsing/Tile/BP_CRightCornerFloor", 'R');
	LoadBPClass(BPFloorMap, "/Game/Collapsing/Tile/BP_CSpeedFloor", 'S');
	LoadBPClass(BPFloorMap, "/Game/Collapsing/Tile/BP_CBrokenFloor", 'B');

	LoadBPClass(GeometryFloorMap, "/Game/Collapsing/Tile/Geometry/GA_CBasicFloor", '0');
	LoadBPClass(GeometryFloorMap, "/Game/Collapsing/Tile/Geometry/GA_CLeftCornerFloor", 'L');
	LoadBPClass(GeometryFloorMap, "/Game/Collapsing/Tile/Geometry/GA_CRightCornerFloor", 'R');


	GeneratedFloorArray.SetNum(MaxTileNum + 1);
	CurrentMapIndex = 0;
}

void UTileManager::LoadBPClass(TMap<uint8, TSubclassOf<AActor>>& TargetMap, const FString& BPPath, uint8 KeyChar)
{
	ConstructorHelpers::FClassFinder<AActor> BPFloor(*BPPath);
	if (BPFloor.Succeeded())
	{
		const int32 Position = BPPath.Find(TEXT("/"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		TargetMap.Emplace(KeyChar, BPFloor.Class);
		UE_LOG(LogTemp, Warning, TEXT("%s Loaded"), *BPPath.RightChop(Position + 1));
	}
}

void UTileManager::ManageTile()
{
	const TCHAR& MapChar = MapString[CurrentMapIndex % MapString.Len()];
	const int32 ArrayIndex = CurrentMapIndex % MaxTileNum;
	//UE_LOG(LogTemp, Warning, TEXT("Current Val : %c"), MapChar)

	DestroyTile(ArrayIndex);
	SpawnTile(MapChar, ArrayIndex);
}

void UTileManager::DestroyTile(const int32 ArrayIndex)
{
	if (CurrentMapIndex >= MaxTileNum && IsValid(GeneratedFloorArray[ArrayIndex]))
	{
		AActor* CurrTile = GeneratedFloorArray[ArrayIndex];
		if (IsValid(CurrTile))
		{
			TCHAR CurrChar = CurrTile->GetName()[4];
			if (CurrChar != 'L' && CurrChar != 'R')
			{
				CurrChar = '0';
			}
			GetWorld()->SpawnActor<AActor>(GeometryFloorMap[CurrChar], CurrTile->GetActorLocation(), CurrTile->GetActorRotation());
			CurrTile->SetActorHiddenInGame(true);
			CurrTile->SetLifeSpan(0.1f);
		}
	}
}

void UTileManager::SpawnTile(const TCHAR& MapChar, const int32 ArrayIndex)
{
	GeneratedFloorArray[ArrayIndex] = GetWorld()->SpawnActor<AActor>(BPFloorMap[MapChar], TileGenTrans.Vector,
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

void UTileManager::InitMaps()
{
	for (int32 It = 0; It != InitTileNum; ++It)
	{
		ManageTile();
	}
	UE_LOG(LogTemp, Warning, TEXT("Init Tiles Generated : %d"), MaxTileNum)
}
