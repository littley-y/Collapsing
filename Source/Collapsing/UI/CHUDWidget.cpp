// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CHUDWidget.h"

#include "Components/TextBlock.h"
#include "Interface/CCharacterWidgetInterface.h"

UCHUDWidget::UCHUDWidget()
{
}

void UCHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	DistanceValue = Cast<UTextBlock>(GetWidgetFromName(TEXT("DistanceValue")));
	DistanceText = Cast<UTextBlock>(GetWidgetFromName(TEXT("DistanceText")));

	ICCharacterWidgetInterface* HUDPawn = Cast<ICCharacterWidgetInterface>(GetOwningPlayerPawn());
	if (HUDPawn != nullptr)
	{
		HUDPawn->SetupCharacterScoreWidget(this);
	}
}

void UCHUDWidget::UpdateDistance(int InValue)
{
	Distance = FMath::CeilToInt(InValue / 100.f);

	DistanceValue->SetText(FText::FromString(FString::FromInt(Distance) + "M"));
}
