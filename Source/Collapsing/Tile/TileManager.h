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
struct FPoolingTile
{
	GENERATED_BODY()

	TArray<TObjectPtr<class ACBasicFloor>> PoolingTiles;
	int SpawnIndex;
	int DestroyIndex;

	FPoolingTile() : SpawnIndex(0), DestroyIndex(0) {}
};

UCLASS()
class COLLAPSING_API UTileManager : public UObject
{
	GENERATED_BODY()

public:
	UTileManager();

	void SetMapString(const FString& InMapString) { MapString = InMapString; }

	static void LoadBPClass(TMap<uint8, TSubclassOf<AActor>>& TargetMap, uint8 KeyChar, const FString& BPPath);

	void InitMaps();

	void SpawnTile();
	void DestroyTile();

protected:
	UPROPERTY()
	TMap<uint8, TSubclassOf<AActor>> BPFloorMap;

	UPROPERTY()
	TMap<uint8, TSubclassOf<AActor>> GeometryFloorMap;

	UPROPERTY()
	TMap<uint8, FPoolingTile> FloorTilePool;

	UPROPERTY()
	TObjectPtr<AActor> InitTilePtr;

	UPROPERTY()
	TSubclassOf<AActor> InitTileClass;

private:
	FString MapString;
	FTileGeneratorTransform TileGenTrans;
	FVector StartPosition;
	float TileSize;

	uint8 MaxTileNum;
	int32 SpawnTileIndex;
	int32 DestroyTileIndex;
};
