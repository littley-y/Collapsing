// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileManager.generated.h"

USTRUCT()
struct FTileGeneratorTransform
{
	GENERATED_BODY()

public:
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

	FPoolingTile() : PoolingTiles(), SpawnIndex(0), DestroyIndex(0) {}
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<uint8, TSubclassOf<AActor>> BPFloorMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<uint8, TSubclassOf<AActor>> GeometryFloorMap;

	TMap<uint8, FPoolingTile> FloorTilePool;

private:
	FString MapString;

	int32 SpawnTileIndex;
	int32 DestroyTileIndex;
	FVector StartPosition;


	float TileSize;
	uint8 MaxTileNum;

	FTileGeneratorTransform TileGenTrans;
};
