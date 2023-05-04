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

	// �ð��� ���� ���� Ÿ���� �����ϴ� �Լ�
	void GenerateTile();
	void SetTileGenerateTimer(const float TargetTime);

protected:
	FTimerHandle TileGenerateTimerHandle;

	virtual void BeginPlay() override;

private:
	float TileGenerateTime;
};
