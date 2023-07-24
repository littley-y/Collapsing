// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CCharacterWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCCharacterWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COLLAPSING_API ICCharacterWidgetInterface
{
	GENERATED_BODY()

public:
	virtual void SetupCharacterHpWidget(class UCUserWidget* InUserWidget) = 0;
	virtual void SetupCharacterTutorialWidget(class UCUserWidget* InUserWidget) = 0;
	virtual void SetupCharacterScoreWidget(class UCUserWidget* InUserWidget) = 0;
};
