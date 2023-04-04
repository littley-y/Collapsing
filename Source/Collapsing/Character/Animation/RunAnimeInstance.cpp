// Fill out your copyright notice in the Description page of Project Settings.

#include "RunAnimeInstance.h"
#include "Collapsing/Character/RunCharacter.h"
#include "GameFramework/PawnMovementComponent.h"

void URunAnimeInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if (IsValid(Pawn)) // Pawn�� nullptr���� �ƴ��� �˻��ϰ� �ڵ����� kill����
	{
		RunCharacter = Cast<ARunCharacter>(Pawn);
	}
}

void URunAnimeInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (RunCharacter != nullptr)
	{
		bIsInAir = RunCharacter->GetMovementComponent()->IsFalling();
		Speed = Pawn->GetVelocity().Size();
	}
}
