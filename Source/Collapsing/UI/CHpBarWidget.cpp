// Fill out your copyright notice in the Description page of Project Settings.

#include "CHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/CCharacterWidgetInterface.h"

UCHpBarWidget::UCHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.f;

}

void UCHpBarWidget::UpdateHpBar(const float NewCurrentHp) const
{
	ensure(MaxHp > 0.f);
	if (HPProgressBar)
	{
		HPProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}

void UCHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HPProgressBar);

	ICCharacterWidgetInterface* CharacterWidget = Cast<ICCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget != nullptr)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}
