// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/CSyncTimerInterface.h"
#include "CollapsingGameMode.generated.h"

UCLASS()
class COLLAPSING_API ACollapsingGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	TObjectPtr<class UTileManager> TileManager;

	ACollapsingGameMode();

	void SetMapBasicString() const;

	void SyncTimer();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileManagerSetting, meta = (AllowPrivateAccess = "true"))
	float TileSpawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileManagerSetting, meta = (AllowPrivateAccess = "true"))
	float TileDestroyTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileManagerSetting, meta = (AllowPrivateAccess = "true"))
	float DestroyDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileManagerSetting, meta = (AllowPrivateAccess = "true"))
	FVector StartPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileManagerSetting, meta = (AllowPrivateAccess = "true"))
	uint8 MaxTileNum;

private:
	UPROPERTY()
	FTimerHandle SpawnTileTimerHandle;

	UPROPERTY()
	FTimerHandle DestroyTileTimerHandle;

	UPROPERTY()
	FTimerHandle CollapsedTimerHandle;

	virtual void BeginPlay() override;

	void SetTileGenerate() const;
	void SetTileDestroy() const;

};
