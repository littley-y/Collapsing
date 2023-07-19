// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CSyncTimerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCSyncTimerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class COLLAPSING_API ICSyncTimerInterface
{
	GENERATED_BODY()

public:
	virtual void SyncTimer() = 0;
};
