// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CGameModeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class COLLAPSING_API ICGameModeInterface
{
	GENERATED_BODY()

public:
	virtual void SyncTimer() = 0;
	virtual void StartStage() = 0;
	virtual void StartArcade() = 0;
	virtual void ExitGame() = 0;
	virtual void RestartGame() = 0;
};