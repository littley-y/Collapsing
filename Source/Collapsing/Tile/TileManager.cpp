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


UTileManager::UTileManager()
{
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

	GeneratedFloorArray.SetNum(MaxTileNum + 1);
	SpawnTileIndex = 0;
	DestroyTileIndex = 0;
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

void UTileManager::ManageTile()
{
	const TCHAR& MapChar = MapString[SpawnTileIndex % MapString.Len()];
	const int32 ArrayIndex = SpawnTileIndex % MaxTileNum;

	//DestroyTile(ArrayIndex);
	SpawnTile(MapChar, ArrayIndex);
}

void UTileManager::DestroyTile(const int32 ArrayIndex)
{
	if (SpawnTileIndex >= MaxTileNum && IsValid(GeneratedFloorArray[ArrayIndex]))
	{
		AActor* CurrTile = GeneratedFloorArray[ArrayIndex];
		if (IsValid(CurrTile))
		{
			TCHAR CurrChar = CurrTile->GetName()[4];
			if (CurrChar != 'L' && CurrChar != 'R' && CurrChar != 'U' && CurrChar != 'D')
			{
				CurrChar = '0';
			}
			//GetWorld()->SpawnActor<AActor>(GeometryFloorMap[CurrChar], CurrTile->GetActorLocation(),
			//                               CurrTile->GetActorRotation());
			// 타일 수명을 설정하지 말고 풀링 형식으로 바꾸기
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
	else if (MapChar == 'U' || MapChar == 'D')
	{
		TileGenTrans.UpdateVectorXY(TileSize);
		TileGenTrans.Vector.Z += MapChar == 'U' ? 200 : -200;
	}
	else
	{
		TileGenTrans.UpdateVectorXY(TileSize);
	}

	SpawnTileIndex++;
	UE_LOG(LogTemp, Warning, TEXT("Floor Gen Location : %s, Yaw : %f"), *TileGenTrans.Vector.ToString(), TileGenTrans.Rotator.Yaw)
}

void UTileManager::InitMaps()
{
	for (int32 It = 0; It != MaxTileNum; ++It)
	{
		ManageTile();
	}
	UE_LOG(LogTemp, Warning, TEXT("Init Tiles Generated : %d"), MaxTileNum)
}
