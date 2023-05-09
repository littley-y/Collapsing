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
	TObjectPtr<class UTileGenerator> TileGenerator;

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

	void DecreseCollapse();

	// 시간에 따라 다음 타일을 생성하는 함수
	void GenerateTile();

};
