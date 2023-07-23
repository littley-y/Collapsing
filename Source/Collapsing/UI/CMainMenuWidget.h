// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CUserWidget.h"
#include "CMainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class COLLAPSING_API UCMainMenuWidget : public UCUserWidget
{
	GENERATED_BODY()

public:
	UCMainMenuWidget();

	void UpdateTextBlock(bool InStatus) const;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	TObjectPtr<class UVerticalBox> TutorialBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	TObjectPtr<class UTextBlock> DoorBlock;
};
