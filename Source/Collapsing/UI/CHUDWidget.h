// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CUserWidget.h"
#include "CHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class COLLAPSING_API UCHUDWidget : public UCUserWidget
{
	GENERATED_BODY()

public:
	UCHUDWidget();

	void UpdateDistance(int InValue);
	FORCEINLINE int32 GetDistance() const { return Distance; }

protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	TObjectPtr<class UTextBlock> DistanceText;

	UPROPERTY()
	TObjectPtr<class UTextBlock> DistanceValue;

	UPROPERTY()
	int32 Distance;
	
};
