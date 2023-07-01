// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CUserWidget.h"
#include "CHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class COLLAPSING_API UCHpBarWidget : public UCUserWidget
{
	GENERATED_BODY()
	
public:
	UCHpBarWidget(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE void SetMaxHp(const float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateHpBar(float NewCurrentHp) const; 

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HPProgressBar;

	UPROPERTY()
	float MaxHp;

	virtual void NativeConstruct() override;
	
};
