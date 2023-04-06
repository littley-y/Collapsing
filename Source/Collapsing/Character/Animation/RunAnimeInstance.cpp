// Fill out your copyright notice in the Description page of Project Settings.

#include "RunAnimeInstance.h"
#include "GameFramework/PawnMovementComponent.h"

void URunAnimeInstance::NativeInitializeAnimation()
{
	if (BasicPawn == nullptr)
	{
		BasicPawn = TryGetPawnOwner();
	}
}

void URunAnimeInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (BasicPawn != nullptr)
	{
		bIsInAir = BasicPawn->GetMovementComponent()->IsFalling();
		Speed = BasicPawn->GetVelocity().Size();
	}
}
