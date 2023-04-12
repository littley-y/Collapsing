// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RunGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class COLLAPSING_API URunGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:


public:
	template <typename T>
	static T* AssetObjectFinder(const FString& FilePath);
};

template <typename T>
T* URunGameInstance::AssetObjectFinder(const FString& FilePath)
{
	ConstructorHelpers::FObjectFinder<T> TargetAsset(*FilePath);
	if (TargetAsset.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Loaded"), *FilePath);
		return TargetAsset.Object;
	}
	return nullptr;
}
