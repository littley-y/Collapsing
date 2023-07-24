// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Stat/CCharacterStatComponent.h"

UCCharacterStatComponent::UCCharacterStatComponent()
{
	MaxHp = 200.f;
	TraveledDistance = 0.f;
	CurrentHp = MaxHp;
	PrimaryComponentTick.bCanEverTick = true;
}

void UCCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHp(MaxHp);
	PrimaryComponentTick.TickInterval = 0.2f;
}

void UCCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	ApplyDamage(1.f);
	AddTraveledDistance();
}

void UCCharacterStatComponent::SetHp(const float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.f, MaxHp);

	OnHpChanged.Broadcast(CurrentHp);
}

void UCCharacterStatComponent::AddTraveledDistance()
{
	const FVector CurrLocation = GetOwner()->GetActorLocation();

	TraveledDistance += FMath::CeilToInt(FVector::Dist(LastLocation, CurrLocation));
	LastLocation = CurrLocation;

	OnDistanceChanged.Broadcast(TraveledDistance);
}

float UCCharacterStatComponent::ApplyDamage(const float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0.f, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast(nullptr);
	}

	return ActualDamage;
}
