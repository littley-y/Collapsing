// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CMapGenerator.generated.h"

/**
 * 
 */
UCLASS()
class COLLAPSING_API UCMapGenerator : public UObject
{
	GENERATED_BODY()

public:
    FString GenerateString(int32 N) const;

    FString GetRandomChar() const;

    FString GetRandomLR() const;

    FString GetRandomDU() const;
};
