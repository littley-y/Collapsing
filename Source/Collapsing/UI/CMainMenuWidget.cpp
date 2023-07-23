// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CMainMenuWidget.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Interface/CCharacterWidgetInterface.h"

UCMainMenuWidget::UCMainMenuWidget()
{
}

void UCMainMenuWidget::UpdateTextBlock(const bool InStatus) const
{
	if (InStatus == true)
	{
		DoorBlock->SetText(FText::FromString("Enter?"));
		DoorBlock->SetVisibility(ESlateVisibility::Visible);
		TutorialBlock->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		DoorBlock->SetVisibility(ESlateVisibility::Hidden);
		TutorialBlock->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TutorialBlock = Cast<UVerticalBox>(GetWidgetFromName(TEXT("Tutorial")));
	DoorBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("DoorOpen")));

	ICCharacterWidgetInterface* CharacterWidget = Cast<ICCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget != nullptr)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}
