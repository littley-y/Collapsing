// Fill out your copyright notice in the Description page of Project Settings.

#include "TileManager.h"
#include "Tile/CBasicFloor.h"
#include "Game/CollapsingGameMode.h"

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
	TileSize = 800.f;

	LoadBPClass(BPFloorMap, '0', "/Game/Collapsing/Tile/BP_CBasicFloor");
	LoadBPClass(BPFloorMap, 'L', "/Game/Collapsing/Tile/BP_CLeftCornerFloor");
	LoadBPClass(BPFloorMap, 'R', "/Game/Collapsing/Tile/BP_CRightCornerFloor");
	LoadBPClass(BPFloorMap, 'B', "/Game/Collapsing/Tile/BP_CBrokenFloor");
	LoadBPClass(BPFloorMap, 'U', "/Game/Collapsing/Tile/BP_CUpRampFloor");
	LoadBPClass(BPFloorMap, 'D', "/Game/Collapsing/Tile/BP_CDownRampFloor");

	LoadBPClass(GeometryFloorMap, '0', "/Game/Collapsing/Tile/Geometry/GA_CBasicFloor");
	LoadBPClass(GeometryFloorMap, 'L', "/Game/Collapsing/Tile/Geometry/GA_CLeftCornerFloor");
	LoadBPClass(GeometryFloorMap, 'R', "/Game/Collapsing/Tile/Geometry/GA_CRightCornerFloor");
	LoadBPClass(GeometryFloorMap, 'U', "/Game/Collapsing/Tile/Geometry/GA_CUpRampFloor");
	LoadBPClass(GeometryFloorMap, 'D', "/Game/Collapsing/Tile/Geometry/GA_CDownRampFloor");

	static ConstructorHelpers::FClassFinder<AActor> InitTileRef(
		TEXT("/Script/Engine.Blueprint'/Game/Collapsing/Tile/BP_CInitTIle.BP_CInitTIle_C'"));
	if (InitTileRef.Succeeded())
	{
		InitTileClass = InitTileRef.Class;
	}
}

void UTileManager::SetMapString(const FString& InMapString)
{
	MapString = *InMapString;
}

void UTileManager::LoadBPClass(TMap<uint32, TSubclassOf<AActor>>& TargetMap, uint32 KeyChar, const FString& BPPath)
{
	ConstructorHelpers::FClassFinder<AActor> BPFloor(*BPPath);
	if (BPFloor.Succeeded())
	{
		const int32 Position = BPPath.Find(TEXT("/"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		TargetMap.Emplace(KeyChar, BPFloor.Class);
		UE_LOG(LogTemp, Warning, TEXT("%s Loaded"), *BPPath.RightChop(Position + 1));
	}
}

void UTileManager::InitMaps(const FVector& InStartPosition, const int32 InMaxTileNum)
{
	StartPosition = InStartPosition;
	MaxTileNum = InMaxTileNum;
	TargetSpawnIndex = 0;
	TargetDestroyIndex = 0;
	bIsSynced = false;

	const FVector InitTileLocation = { -820.f, 0.f, -40.f };
	InitTilePtr = UObject::GetWorld()->SpawnActor<AActor>(InitTileClass, InitTileLocation, FRotator::ZeroRotator);

	int TileTypes[] = { '0', 'L', 'R', 'B', 'U', 'D' };
	for (const auto& TileType : TileTypes)
	{
		FloorTilePool.Add(TileType);
		auto& CurrPoolingTiles = FloorTilePool[TileType].PoolingTiles;
		CurrPoolingTiles.SetNum(MaxTileNum);
		for (size_t i = 0; i < MaxTileNum; i++)
		{
			CurrPoolingTiles[i] = GetWorld()->SpawnActor<ACBasicFloor>(BPFloorMap[TileType], StartPosition,
				FRotator::ZeroRotator);
			CurrPoolingTiles[i]->SetActorHiddenInGame(true);
			CurrPoolingTiles[i]->SetActorEnableCollision(false);
		}
		BPFloorMap.Remove(TileType);
	}

	for (int32 It = 0; It != MaxTileNum / 2; ++It)
	{
		SpawnTile();
	}
	UE_LOG(LogTemp, Warning, TEXT("Init Tiles Generated : %d"), MaxTileNum / 2);
}

void UTileManager::SpawnTile()
{
	const TCHAR MapChar = MapString[TargetSpawnIndex % MapString.Len()];
	if (MapChar != '0' && MapChar != 'L' && MapChar != 'R' && MapChar != 'U' && MapChar != 'D' && MapChar != 'B')
	{
		return;
	}

	FTilePool& SelectedTilePool = FloorTilePool[MapChar];
	uint32& TargetIndex = SelectedTilePool.SpawnedIndex;
	ACBasicFloor* TargetTile = SelectedTilePool.PoolingTiles[TargetIndex];
	if (IsValid(TargetTile) == false)
	{
		return;
	}

	TargetTile->SetActorLocationAndRotation(TileGenTrans.Vector, TileGenTrans.Rotator);
	TargetTile->ActivateFloor();

	TargetIndex = (TargetIndex + 1) % MaxTileNum;
	TargetSpawnIndex++;

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
	else if (MapChar == '0')
	{
		TileGenTrans.UpdateVectorXY(TileSize);
	}
	else
	{
		return;
	}

	if (bIsSynced == false && TargetSpawnIndex - TargetDestroyIndex == MaxTileNum)
	{
		ICSyncTimerInterface* GameModeSyncTimerInterface = Cast<ICSyncTimerInterface>(GetWorld()->GetAuthGameMode());
		if (GameModeSyncTimerInterface != nullptr)
		{
			GameModeSyncTimerInterface->SyncTimer();
			bIsSynced = true;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Curr Indexs : %d %d"), TargetSpawnIndex, TargetDestroyIndex)
}

void UTileManager::DestroyTile()
{
	const TCHAR MapChar = MapString[TargetDestroyIndex % MapString.Len()];
	if (MapChar != '0' && MapChar != 'L' && MapChar != 'R' && MapChar != 'U' && MapChar != 'D' && MapChar != 'B')
	{
		return;
	}

	FTilePool& SelectedTilePool = FloorTilePool[MapChar];
	uint32& TargetIndex = SelectedTilePool.DestroyedIndex;
	ACBasicFloor* TargetTile = SelectedTilePool.PoolingTiles[TargetIndex];
	if (IsValid(TargetTile) == false)
	{
		return;
	}

	if (TargetDestroyIndex == 0 && IsValid(InitTilePtr))
	{
		InitTilePtr->SetLifeSpan(0.1f);
	}

	const FTransform CurrTransform = TargetTile->GetActorTransform();
	AActor* DestroyingFloor = GetWorld()->SpawnActor<AActor>(GeometryFloorMap[MapChar], CurrTransform);
	DestroyingFloor->SetLifeSpan(1.f);

	TargetTile->DeactivateFloor();
	TargetIndex = (TargetIndex + 1) % MaxTileNum;
	TargetDestroyIndex++;
}

