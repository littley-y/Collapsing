// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileGenerator.generated.h"

enum EFloorType
{
	Basic = 0,
	LeftCorner,
	RightCorner,
	Speed,
	Up,
	Do
};

USTRUCT()
struct FTileGeneratorTransform
{
	GENERATED_BODY()

	FVector Vector;
	FRotator Rotator;

	FTileGeneratorTransform() : Vector(-800.f, 0.f, 0.f), Rotator(0.f, 0.f, 0.f) {}

	void UpdateVectorXY(const float InValue);
	void UpdateVectorZ(const float InValue);
};

UCLASS()
class COLLAPSING_API UTileGenerator : public UObject
{
	GENERATED_BODY()

public:
	UTileGenerator();

	void SetMapString(const FString& InMapString) { MapString = InMapString; }

	void LoadBPFloor(const FString& BPPath);

	void InitMaps();

	void SpawnFloorTile();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<AActor>> BPFloorArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AActor>> GeneratedFloorArray;

private:
	FString MapString;

	int32 CurrentMapIndex;

	static constexpr float TileSize = 400.f;

	static constexpr uint8 InitTileNum = 6;
	static constexpr uint8 MaxTileNum = 10;

	FTileGeneratorTransform TileGenTrans;
};
