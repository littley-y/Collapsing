// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileGenerator.generated.h"

UENUM()
enum EFloorType
{
	Basic = 0,
	LeftCorner,
	RightCorner,
	Speed
};

USTRUCT()
struct FTileGeneratorTransform
{
	GENERATED_BODY()

	FVector Vector;
	FRotator Rotator;

	FTileGeneratorTransform() : Vector(0.f, 0.f, 0.f), Rotator(0.f, 0.f, 0.f) {}

	void MoveVector(const float InValue);
};

UCLASS()
class COLLAPSING_API UTileGenerator : public UObject
{
	GENERATED_BODY()

public:
	UTileGenerator();

	void SetMapString(const FString& InMapString) { MapString = InMapString; }

	void AddBPFloor(const FString& BPPath);

	void InitMaps();

	void AddFloorTile();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<AActor>> BPFloorArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AActor>> GeneratedFloorArray;

private:
	FString MapString;
	int32 CurrentMapIndex = 0;

	static constexpr uint16 MaxTileNum = 10;

	FTileGeneratorTransform TileGenTrans;
};
