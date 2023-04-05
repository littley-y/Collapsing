// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CollapsingGameModeBase.generated.h"

UCLASS()
class COLLAPSING_API ACollapsingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UTileGenerator> TileGenerator;

	ACollapsingGameModeBase();

protected:

	virtual void BeginPlay() override;
};
