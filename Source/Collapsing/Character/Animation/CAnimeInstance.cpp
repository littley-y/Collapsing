// Fill out your copyright notice in the Description page of Project Settings.

#include "CAnimeInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/CCharacter.h"

UCAnimeInstance::UCAnimeInstance()
{
	MovingThreshold = 3.f;
	JumpingThreshold = 100.f;
}

void UCAnimeInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACCharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
		CharacterMaxHp = Owner->GetCharacterMaxHp();
	}
}

void UCAnimeInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement && Owner)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshold;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);
		CharacterHp = Owner->GetCharacterHp();
	}
}
