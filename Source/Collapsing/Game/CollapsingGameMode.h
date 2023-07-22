// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/CGameModeInterface.h"
#include "CollapsingGameMode.generated.h"

UCLASS()
class COLLAPSING_API ACollapsingGameMode : public AGameModeBase, public ICGameModeInterface
{
	GENERATED_BODY()

public:
	ACollapsingGameMode();

// Map Section
public:
	void SetStageMapString();
	void SetArcadeMapString() const;

protected:
	UPROPERTY()
	TObjectPtr<class UCMapGenerator> MapGenerator;

	UPROPERTY()
	FString StageMapString;

	UPROPERTY()
	FString ArcadeMapString;

public:
	virtual void SetTimer(int32 InTimerType) override;

	virtual void StartGame(const FString MapType) override;
	virtual void ExitGame() override;
	virtual void RestartGame() override;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY()
	TObjectPtr<class UCTileManager> TileManager;

	UPROPERTY()
	TObjectPtr<class AActor> InitTile;

	UPROPERTY()
	TSubclassOf<class AActor> BP_InitTile;

	UPROPERTY()
	TObjectPtr<class AActor> GeometryInitTile;

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
	FTimerHandle SpawnTileTimerHandle;

	UPROPERTY()
	FTimerHandle DestroyTileTimerHandle;

	UPROPERTY()
	FTimerHandle CollapsedTimerHandle;

	virtual void BeginPlay() override;

	void SetTileGenerate() const;
	void SetTileDestroy() const;

};
