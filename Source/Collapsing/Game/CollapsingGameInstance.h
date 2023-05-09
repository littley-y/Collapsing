// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CollapsingGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class COLLAPSING_API UCollapsingGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	const float GetCollapsed() const { return Collapsed; }
	void SetCollapsed(const float InValue) { Collapsed = InValue; }

private:
	float Collapsed;
};
