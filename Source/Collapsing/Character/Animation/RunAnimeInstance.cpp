// Fill out your copyright notice in the Description page of Project Settings.

#include "RunAnimeInstance.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/Character.h"

void URunAnimeInstance::NativeInitializeAnimation()
{
	if (BasicPawn == nullptr)
	{
		BasicPawn = TryGetPawnOwner();
	}
}

void URunAnimeInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	ACharacter* MyCharacter = Cast<ACharacter>(BasicPawn);
	if (IsValid(MyCharacter))
	{
		bIsJumping = MyCharacter->GetMovementComponent()->IsFalling() && MyCharacter->JumpCurrentCount;
		bIsFalling = MyCharacter->GetMovementComponent()->IsFalling() && MyCharacter->JumpCurrentCount == 0;
		Speed = MyCharacter->GetVelocity().Size();
	}
}
