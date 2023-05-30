// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CollapsingGameMode.generated.h"

UCLASS()
class COLLAPSING_API ACollapsingGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UTileManager> TileGenerator;

	ACollapsingGameMode();

	void SetMapBasicString() const;

	void SetTileGenerateTimer(const float TargetTime);

private:
	float TileGenerateTime;

	UPROPERTY()
	FTimerHandle TileGenerateTimerHandle;

	UPROPERTY()
	FTimerHandle CollapsedTimerHandle;

	virtual void BeginPlay() override;

	void CallTileManager() const;

};
