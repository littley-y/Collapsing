// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class COLLAPSING_API UCHpBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCHpBarWidget(const FObjectInitializer& ObjectInitializer);

	void UpdateHpBar(float NewCurrentHp); 

protected:
	FORCEINLINE void SetHp(float NewMaxHp) { MaxHp = NewMaxHp; }

	virtual void NativeConstruct();

private:
	float MaxHp;

	UPROPERTY()
	class UProgressBar* HPProgressBar;
};
