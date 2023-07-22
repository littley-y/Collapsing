// Fill out your copyright notice in the Description page of Project Settings.


#include "CMapGenerator.h"

FString UCMapGenerator::GenerateString(const int32 N) const
{
    FString Output;

    for (int32 i = 0; i < N; i++)
    {
        if (i == 0)
			Output += FMath::RandBool() ? "0" : "B";
        Output += GetRandomChar();
        Output += GetRandomChar();
        Output += GetRandomChar();
        Output += GetRandomChar();
        Output += GetRandomChar();
        Output += GetRandomChar();
        Output += GetRandomChar();
        Output += GetRandomLR();
        Output += GetRandomDU();
        Output += GetRandomLR();
    }
    return Output;
}

FString UCMapGenerator::GetRandomChar() const
{
    FString Chars = "0OB";
    const int32 RandomIndex = FMath::RandRange(0, 2);
    return FString::Printf(TEXT("%c"), Chars[RandomIndex]);
}

FString UCMapGenerator::GetRandomLR() const
{
    FString Chars = "LR";
    const int32 RandomIndex = FMath::RandRange(0, 1);
    return FString::Printf(TEXT("%c"), Chars[RandomIndex]);
}

FString UCMapGenerator::GetRandomDU() const
{
    FString Chars = "DU";
    const int32 randomIndex = FMath::RandRange(0, 1);
    return FString::Printf(TEXT("%c"), Chars[randomIndex]);
}
