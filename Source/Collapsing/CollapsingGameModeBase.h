// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CollapsingGameModeBase.generated.h"

/**
 * 
 */

UENUM()
enum EFloorType
{
	Basic = 0,
	LeftCorner,
	RightCorner
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
	void AddFloorTile(FString MapName);


protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FloorArray")
	TArray<TSubclassOf<AActor>> FloorArray;
};
