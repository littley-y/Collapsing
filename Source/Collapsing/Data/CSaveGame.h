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
	void SaveDistance(int32 InDistance);
	const TArray<int32>& GetSavedDistances() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SaveData)
	TArray<int32> Distances;

	UPROPERTY(VisibleAnywhere, Category = SaveData)
	FString PlayerName;
};
