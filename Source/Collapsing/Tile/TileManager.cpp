// Fill out your copyright notice in the Description page of Project Settings.

#include "TileManager.h"
#include "Tile/CBasicFloor.h"

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


UTileManager::UTileManager()
{
	SpawnTileIndex = 0;
	DestroyTileIndex = 0;
	TileSize = 800.f;
	MaxTileNum = 40;
	StartPosition = { -1000.f, 0.f, 1000.f };

	LoadBPClass(BPFloorMap, '0', "/Game/Collapsing/Tile/BP_CBasicFloor");
	LoadBPClass(BPFloorMap, 'L', "/Game/Collapsing/Tile/BP_CLeftCornerFloor");
	LoadBPClass(BPFloorMap, 'R', "/Game/Collapsing/Tile/BP_CRightCornerFloor");
	LoadBPClass(BPFloorMap, 'S', "/Game/Collapsing/Tile/BP_CSpeedFloor");
	LoadBPClass(BPFloorMap, 'B', "/Game/Collapsing/Tile/BP_CBrokenFloor");
	LoadBPClass(BPFloorMap, 'U', "/Game/Collapsing/Tile/BP_CUpRampFloor");
	LoadBPClass(BPFloorMap, 'D', "/Game/Collapsing/Tile/BP_CDownRampFloor");

	LoadBPClass(GeometryFloorMap, '0', "/Game/Collapsing/Tile/Geometry/GA_CBasicFloor");
	LoadBPClass(GeometryFloorMap, 'L', "/Game/Collapsing/Tile/Geometry/GA_CLeftCornerFloor");
	LoadBPClass(GeometryFloorMap, 'R', "/Game/Collapsing/Tile/Geometry/GA_CRightCornerFloor");
	LoadBPClass(GeometryFloorMap, 'U', "/Game/Collapsing/Tile/Geometry/GA_CUpRampFloor");
	LoadBPClass(GeometryFloorMap, 'D', "/Game/Collapsing/Tile/Geometry/GA_CDownRampFloor");
}

void UTileManager::LoadBPClass(TMap<uint8, TSubclassOf<AActor>>& TargetMap, uint8 KeyChar, const FString& BPPath)
{
	ConstructorHelpers::FClassFinder<AActor> BPFloor(*BPPath);
	if (BPFloor.Succeeded())
	{
		const int32 Position = BPPath.Find(TEXT("/"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		TargetMap.Emplace(KeyChar, BPFloor.Class);
		UE_LOG(LogTemp, Warning, TEXT("%s Loaded"), *BPPath.RightChop(Position + 1));
	}
}

void UTileManager::InitMaps()
{
	int TileNames[] = { '0', 'L', 'R', 'S', 'B', 'U', 'D' };
	for (const auto& TileName : TileNames)
	{
		FloorTilePool.Add(TileName);
		TArray<TObjectPtr<ACBasicFloor>>& CurrPoolingTiles = FloorTilePool[TileName].PoolingTiles;
		CurrPoolingTiles.SetNum(MaxTileNum);
		for (size_t i = 0; i < MaxTileNum; i++)
		{
			CurrPoolingTiles[i] = GetWorld()->SpawnActor<ACBasicFloor>(BPFloorMap[TileName], StartPosition, FRotator::ZeroRotator);
			CurrPoolingTiles[i]->SetActorHiddenInGame(true);
		}
	}

	for (int32 It = 0; It != MaxTileNum / 2; ++It)
	{
		SpawnTile();
	}
	UE_LOG(LogTemp, Warning, TEXT("Init Tiles Generated : %d"), MaxTileNum / 2)
}

void UTileManager::SpawnTile()
{
	const TCHAR MapChar = MapString[SpawnTileIndex % MapString.Len()];
	int32& ArrayIndex = FloorTilePool[MapChar].SpawnIndex;
	ACBasicFloor* CurrTile = FloorTilePool[MapChar].PoolingTiles[ArrayIndex];

	if (IsValid(CurrTile) == false)
	{
		return;
	}

	CurrTile->SetActorLocationAndRotation(TileGenTrans.Vector, TileGenTrans.Rotator);
	CurrTile->ActiveFloor();

	ArrayIndex = (ArrayIndex + 1) % MaxTileNum;
	SpawnTileIndex++;

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
	else if (MapChar == 'U' || MapChar == 'D')
	{
		TileGenTrans.UpdateVectorXY(TileSize);
		TileGenTrans.Vector.Z += MapChar == 'U' ? 215 : -210;
	}
	else
	{
		TileGenTrans.UpdateVectorXY(TileSize);
	}

	UE_LOG(LogTemp, Warning, TEXT("Floor Gen Location : %s, Yaw : %f"), *TileGenTrans.Vector.ToString(), TileGenTrans.Rotator.Yaw)
}

void UTileManager::DestroyTile()
{
	const TCHAR MapChar = MapString[DestroyTileIndex % MapString.Len()];
	int32& ArrayIndex = FloorTilePool[MapChar].DestroyIndex;

	ACBasicFloor* CurrTile = FloorTilePool[MapChar].PoolingTiles[ArrayIndex];
	if (IsValid(CurrTile) == false)
	{
		return;
	}

	// 같은 위치에 액터 소환하면서 붕괴 효과 주기
	TCHAR CurrChar = CurrTile->GetName()[4];
	if (CurrChar != 'L' && CurrChar != 'R' && CurrChar != 'U' && CurrChar != 'D')
	{
		CurrChar = '0';
	}
	FTransform CurrTransform(CurrTile->GetActorTransform());
	AActor* DestroyingFloor = GetWorld()->SpawnActor<AActor>(GeometryFloorMap[CurrChar], CurrTransform);
	DestroyingFloor->SetLifeSpan(1.f);

	CurrTile->DeactiveFloor();
	ArrayIndex = (ArrayIndex + 1) % MaxTileNum;
	DestroyTileIndex++;
}

