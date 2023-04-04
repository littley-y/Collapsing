// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CollapsingGameModeBase.generated.h"

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
class COLLAPSING_API ACollapsingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACollapsingGameModeBase();

	void AddBPFloor(FString BPPath);

	UFUNCTION()
	void GenerateMaps();

	UFUNCTION()
	void AddFloorTile();

private:
	FString BasicMapString;

	FTileGeneratorTransform TileGenTrans;

	int32 CurrentMapIndex;
	
	static constexpr uint16 MaxTileNum = 10;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<AActor>> BPFloorArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AActor>> GeneratedFloorArray;
};
