// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CCharacterControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCCharacterControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COLLAPSING_API ICCharacterControllerInterface
{
	GENERATED_BODY()

public:
	virtual void GameOver() = 0;
};
