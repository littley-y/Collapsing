// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Stat/CCharacterStatComponent.h"

UCCharacterStatComponent::UCCharacterStatComponent()
{
	MaxHp = 100.f;
	CurrentHp = MaxHp;
	PrimaryComponentTick.bCanEverTick = true;
}

void UCCharacterStatComponent::SetHp(const float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.f, MaxHp);

	OnHpChanged.Broadcast(CurrentHp);
}

void UCCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	ApplyDamage(.2f);
}

void UCCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHp(MaxHp);
}

float UCCharacterStatComponent::ApplyDamage(const float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0.f, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

