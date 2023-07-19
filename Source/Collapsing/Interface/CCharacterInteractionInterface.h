// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CCharacterInteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCCharacterInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COLLAPSING_API ICCharacterInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void HitBySomething() = 0;
	virtual void EarnHpUpItem() = 0;

	virtual void SetTurnStatus(bool InStatus) = 0;
	virtual bool GetTurnStatus() const = 0;

	virtual void Death() = 0;
};
