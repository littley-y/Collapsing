// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/CSyncTimerInterface.h"
#include "CollapsingGameMode.generated.h"

UCLASS()
class COLLAPSING_API ACollapsingGameMode : public AGameModeBase, public ICSyncTimerInterface
{
	GENERATED_BODY()

public:
	ACollapsingGameMode();

	void SetMapBasicString();

	virtual void SyncTimer() override;

protected:
	UPROPERTY()
	TObjectPtr<class UTileManager> TileManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileManagerSetting, meta = (AllowPrivateAccess = "true"))
	float TileSpawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileManagerSetting, meta = (AllowPrivateAccess = "true"))
	float TileDestroyTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileManagerSetting, meta = (AllowPrivateAccess = "true"))
	float DestroyDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileManagerSetting, meta = (AllowPrivateAccess = "true"))
	int32 MaxTileNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileManagerSetting, meta = (AllowPrivateAccess = "true"))
	FVector StartPosition;

private:
	UPROPERTY()
	FString MapString;

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
