// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTileManager.generated.h"

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
class COLLAPSING_API UCTileManager : public UObject
{
	GENERATED_BODY()

public:
	UCTileManager();

// Map Section
public:
	void SetMapString(const FString& InMapString);
	void InitMaps(const FVector& InStartPosition, const int32 InMaxTileNum);

private:
	UPROPERTY()
	FString MapString;
	FVector StartPosition;

// Tile Section
public:
	static void LoadBPClass(TMap<uint32, TSubclassOf<AActor>>& TargetMap, uint32 KeyChar, const FString& BPPath);

	void SpawnTile();
	void DestroyTile();

protected:
	UPROPERTY()
	TMap<uint32, TSubclassOf<AActor>> BPFloorMap;

	UPROPERTY()
	TMap<uint32, TSubclassOf<AActor>> GeometryFloorMap;

	UPROPERTY()
	TMap<uint32, FTilePool> FloorTilePool;

	UPROPERTY()
	TObjectPtr<USoundBase> CollapseSound;

private:
	FTileGeneratorTransform TileGenTrans;

	int32 MaxTileNum;

	uint32 TargetSpawnIndex;
	uint32 TargetDestroyIndex;
	float TileSize;
	bool bIsSynced;
};
