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

UCLASS()
class COLLAPSING_API UTileManager : public UObject
{
	GENERATED_BODY()

public:
	UTileManager();

	void SetMapString(const FString& InMapString) { MapString = InMapString; }

	static void LoadBPClass(TMap<uint8, TSubclassOf<AActor>>& TargetMap, uint8 KeyChar, const FString& BPPath);

	void InitMaps();

	void ManageTile();
	void DestroyTile(const int32 ArrayIndex);
	void SpawnTile(const TCHAR& MapChar, const int32 ArrayIndex);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<uint8, TSubclassOf<AActor>> BPFloorMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<uint8, TSubclassOf<AActor>> GeometryFloorMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AActor>> GeneratedFloorArray;

private:
	FString MapString;

	int32 CurrentMapIndex;

	static constexpr float TileSize = 800.f;
	static constexpr uint8 MaxTileNum = 10;

	FTileGeneratorTransform TileGenTrans;
};
