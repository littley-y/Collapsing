// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class COLLAPSING_API UCSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UCSaveGame();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 Distance;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString PlayerName;
};
