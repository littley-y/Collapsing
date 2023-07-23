// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CCharacterInteractionInterface.generated.h"

UENUM()
enum class EDoorType : uint8
{
	Stage,
	Arcade,
	Quit,
	None
};

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

	virtual void ChangeCanDoorOpen(EDoorType InDoorType) = 0;
	virtual EDoorType GetWhichDoorCanOpen() = 0;

	virtual void SetCanTurn(bool InStatus) = 0;
	virtual bool GetCanTurn() const = 0;
	
	virtual void Death(AActor* CausedActor) = 0;
};
