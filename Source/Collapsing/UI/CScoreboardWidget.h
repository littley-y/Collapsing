// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CUserWidget.h"
#include "CScoreboardWidget.generated.h"

/**
 * 
 */
UCLASS()
class COLLAPSING_API UCScoreboardWidget : public UCUserWidget
{
	GENERATED_BODY()

public:
	UCScoreboardWidget();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	TArray<TObjectPtr<class UTextBlock>> SavedScores;
};
