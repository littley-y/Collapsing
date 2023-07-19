// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileManager.generated.h"

USTRUCT()
struct FTileGeneratorTransform
{
	GENERATED_BODY()

	FVector Vector;
	FRotator Rotator;

	FTileGeneratorTransform() : Vector(-800.f, 0.f, 0.f), Rotator(0.f, 0.f, 0.f) {}

	void UpdateVectorXY(const float InValue);
};

USTRUCT()
struct FTilePool
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TObjectPtr<class ACBasicFloor>> PoolingTiles;

	uint32 SpawnedIndex;
	uint32 DestroyedIndex;

	FTilePool() : SpawnedIndex(0), DestroyedIndex(0) {}
};

UCLASS()
class COLLAPSING_API UTileManager : public UObject
{
	GENERATED_BODY()

public:
	UTileManager();

	void SetMapString(const FString& InMapString);

	static void LoadBPClass(TMap<uint32, TSubclassOf<AActor>>& TargetMap, uint32 KeyChar, const FString& BPPath);

	void InitMaps(const FVector& InStartPosition, const int32 InMaxTileNum);

	void SpawnTile();
	void DestroyTile();

protected:
	UPROPERTY()
	TMap<uint32, TSubclassOf<AActor>> BPFloorMap;

	UPROPERTY()
	TMap<uint32, TSubclassOf<AActor>> GeometryFloorMap;

	UPROPERTY()
	TObjectPtr<AActor> InitTilePtr;

	UPROPERTY()
	TSubclassOf<AActor> InitTileClass;

	UPROPERTY()
	TMap<uint32, FTilePool> FloorTilePool;

private:
	FString MapString;

	FTileGeneratorTransform TileGenTrans;

	FVector StartPosition;
	int32 MaxTileNum;

	uint32 TargetSpawnIndex;
	uint32 TargetDestroyIndex;
	float TileSize;
	bool bIsSynced;
};
