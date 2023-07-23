// Fill out your copyright notice in the Description page of Project Settings.

#include "CTileManager.h"
#include "Tile/CBasicFloor.h"
#include "Game/CollapsingGameMode.h"
#include "Kismet/GameplayStatics.h"

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

UCTileManager::UCTileManager()
{
	TileSize = 800.f;
	TargetSpawnIndex = 0;
	TargetDestroyIndex = 0;
	bIsSynced = false;

	TTuple<uint32, FString> BPFloorMapPairs[] = {
		{'0', "/Game/Collapsing/Tile/BP_CBasicFloor"}, {'L', "/Game/Collapsing/Tile/BP_CLeftCornerFloor"},
		{'R', "/Game/Collapsing/Tile/BP_CRightCornerFloor"}, {'B', "/Game/Collapsing/Tile/BP_CBrokenFloor"},
		{'U', "/Game/Collapsing/Tile/BP_CUpRampFloor"}, {'D', "/Game/Collapsing/Tile/BP_CDownRampFloor"},
		{'O', "/Game/Collapsing/Tile/BP_CObstacleFloor"}
	};

	for (const auto& BPFloorMapPair : BPFloorMapPairs)
	{
		LoadBPClass(BPFloorMap, BPFloorMapPair.Key, BPFloorMapPair.Value);
	}

	TTuple<uint32, FString> GeometryFloorMapPairs[] = {
		{'0', "/Game/Collapsing/Tile/Geometry/GA_CBasicFloor"},
		{'L', "/Game/Collapsing/Tile/Geometry/GA_CLeftCornerFloor"},
		{'R', "/Game/Collapsing/Tile/Geometry/GA_CRightCornerFloor"},
		{'U', "/Game/Collapsing/Tile/Geometry/GA_CUpRampFloor"},
		{'D', "/Game/Collapsing/Tile/Geometry/GA_CDownRampFloor"},
		{'B', "/Game/Collapsing/Tile/Geometry/GA_CBrokenFloor"}
	};

	for (const auto& GeometryFloorMapPair : GeometryFloorMapPairs)
	{
		LoadBPClass(GeometryFloorMap, GeometryFloorMapPair.Key, GeometryFloorMapPair.Value);
	}

	CollapseSound = CreateDefaultSubobject<USoundBase>(TEXT("CollapseSound"));
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundRef(
		TEXT("/Script/Engine.SoundCue'/Game/_GameAssets/Audio/Explosion_Cue.Explosion_Cue'"));
	if (IsValid(SoundRef.Object))
	{
		CollapseSound = SoundRef.Object;
	}
}

void UCTileManager::SetMapString(const FString& InMapString)
{
	MapString = *InMapString;
}

void UCTileManager::LoadBPClass(TMap<uint32, TSubclassOf<AActor>>& TargetMap, uint32 KeyChar, const FString& BPPath)
{
	ConstructorHelpers::FClassFinder<AActor> BPFloor(*BPPath);
	if (BPFloor.Succeeded())
	{
		const int32 Position = BPPath.Find(TEXT("/"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		TargetMap.Emplace(KeyChar, BPFloor.Class);
		UE_LOG(LogTemp, Warning, TEXT("%s Loaded"), *BPPath.RightChop(Position + 1));
	}
}

void UCTileManager::InitMaps(const FVector& InStartPosition, const int32 InMaxTileNum)
{
	StartPosition = InStartPosition;
	MaxTileNum = InMaxTileNum;

	uint32 TileTypes[] = { '0', 'L', 'R', 'B', 'U', 'D', 'O' };
	for (const auto& TileType : TileTypes)
	{
		FloorTilePool.Add(TileType);
		TArray<TObjectPtr<ACBasicFloor>>& CurrPoolingTiles = FloorTilePool[TileType].PoolingTiles;
		CurrPoolingTiles.SetNum(MaxTileNum);
		for (int32 i = 0; i < MaxTileNum; i++)
		{
			CurrPoolingTiles[i] = GetWorld()->SpawnActor<ACBasicFloor>(BPFloorMap[TileType], StartPosition,
				FRotator::ZeroRotator);
			CurrPoolingTiles[i]->SetActorHiddenInGame(true);
			CurrPoolingTiles[i]->SetActorEnableCollision(false);
			CurrPoolingTiles[i]->SetActorTickEnabled(false);
		}
	}

	for (int32 It = 0; It != MaxTileNum / 2; ++It)
	{
		SpawnTile();
	}
}

void UCTileManager::SpawnTile()
{
	const TCHAR MapChar = MapString[TargetSpawnIndex % MapString.Len()];
	if (MapChar != '0' && MapChar != 'L' && MapChar != 'R' && MapChar != 'U' && 
		MapChar != 'D' && MapChar != 'B' && MapChar != 'O')
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
	else
	{
		TileGenTrans.UpdateVectorXY(TileSize);
	}

	if (bIsSynced == false && TargetSpawnIndex - TargetDestroyIndex == MaxTileNum)
	{
		ICGameModeInterface* GameModeSyncTimerInterface = Cast<ICGameModeInterface>(GetWorld()->GetAuthGameMode());
		if (GameModeSyncTimerInterface != nullptr)
		{
			GameModeSyncTimerInterface->SetTimer(0);
			bIsSynced = true;
		}
	}
}

void UCTileManager::DestroyTile()
{
	TCHAR MapChar = MapString[TargetDestroyIndex % MapString.Len()];
	if (MapChar != '0' && MapChar != 'L' && MapChar != 'R' && MapChar != 'U' && MapChar != 'D' && MapChar != 'B' &&
		MapChar != 'O')
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

	if (MapChar == 'O')
	{
		MapChar = '0';
	}
	const FTransform CurrTransform = TargetTile->GetActorTransform();
	AActor* DestroyingFloor = GetWorld()->SpawnActor<AActor>(GeometryFloorMap[MapChar], CurrTransform);
	if (IsValid(CollapseSound))
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), CollapseSound, CurrTransform.GetLocation(), CurrTransform.Rotator(), 0.5f, 0.5f);
	}
	DestroyingFloor->SetLifeSpan(3.f);

	TargetTile->DeactivateFloor();
	TargetIndex = (TargetIndex + 1) % MaxTileNum;
	TargetDestroyIndex++;
}
