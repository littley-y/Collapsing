// Fill out your copyright notice in the Description page of Project Settings.

#include "RunAnimeInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/RunCharacter.h"

URunAnimeInstance::URunAnimeInstance()
{
	MovingThreshold = 3.f;
	JumpingThreshold = 100.f;
}

void URunAnimeInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ARunCharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void URunAnimeInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement && Owner)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshold;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);
		CharacterHp = Owner->GetHp();
	}
}
