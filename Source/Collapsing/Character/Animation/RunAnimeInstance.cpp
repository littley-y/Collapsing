// Fill out your copyright notice in the Description page of Project Settings.


#include "RunAnimeInstance.h"

#include "GameFramework/PawnMovementComponent.h"

void URunAnimeInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if (IsValid(Pawn)) // Pawn이 nullptr인지 아닌지 검사하고 자동으로 kill까지
	{
		MovementComponent = Pawn->GetMovementComponent();
	}
}

void URunAnimeInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (Pawn && MovementComponent)
	{
		bIsInAir = MovementComponent->IsFalling();
		Speed = Pawn->GetVelocity().Size();

	/*	Speed = UKismetMathLibrary::VSize(Pawn->GetVelocity()) 와 동일
	 *	FMath도 사용 가능하다 충분히
		블루프린트를 C++로 바꿀 때 UKisemtMathLibrary를 참조하면 좋다. */
	}
}
