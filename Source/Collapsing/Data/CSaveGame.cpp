// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/CSaveGame.h"

UCSaveGame::UCSaveGame()
{
	PlayerName = "Default";
}

void UCSaveGame::SaveDistance(int32 InDistance)
{
	Distances.Add(InDistance);
	Distances.Sort([](const int32& A, const int32& B) { return A > B; });
	if (Distances.Num() == 9)
	{
		Distances.Pop();
	}
}

const TArray<int32>& UCSaveGame::GetSavedDistances() const
{
	return Distances;
}
