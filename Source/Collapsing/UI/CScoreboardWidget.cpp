// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CScoreboardWidget.h"
#include "Components/TextBlock.h"
#include "Data/CSaveGame.h"
#include "Kismet/GameplayStatics.h"

UCScoreboardWidget::UCScoreboardWidget()
{
}

void UCScoreboardWidget::UpdateScoreboard(const TArray<int32>& InSavedScores)
{
	for (int32 i = 0; i < InSavedScores.Num(); ++i)
	{
		FString BlockText = FString::Printf(TEXT("%d. %dM"), i + 1, InSavedScores[i]);
		SavedScores[i]->SetText(FText::FromString(BlockText));
	}
}

void UCScoreboardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SavedScores.SetNum(8);
	for (int32 i = 0; i < 8; ++i)
	{
		FString BlockName = "Score_";
		BlockName += FString::FromInt(i);
		SavedScores[i] = Cast<UTextBlock>(GetWidgetFromName(*BlockName));
	}

	const UCSaveGame* SaveGameInstance = Cast<UCSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Player0"), 0));
	if (IsValid(SaveGameInstance))
	{
		UpdateScoreboard(SaveGameInstance->GetSavedDistances());
	}
}
