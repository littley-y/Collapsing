// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CWidgetComponent.h"
#include "CUserWidget.h"

void UCWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UCUserWidget* CUserWidget = Cast<UCUserWidget>(GetWidget());
	if (nullptr != CUserWidget)
	{
		CUserWidget->SetOwningActor(GetOwner());
	}
}
