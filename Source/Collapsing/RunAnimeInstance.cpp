// Fill out your copyright notice in the Description page of Project Settings.


#include "RunAnimeInstance.h"

#include "GameFramework/PawnMovementComponent.h"

void URunAnimeInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if (IsValid(Pawn)) // Pawn�� nullptr���� �ƴ��� �˻��ϰ� �ڵ����� kill����
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

	/*	Speed = UKismetMathLibrary::VSize(Pawn->GetVelocity()) �� ����
	 *	FMath�� ��� �����ϴ� �����
		�������Ʈ�� C++�� �ٲ� �� UKisemtMathLibrary�� �����ϸ� ����. */
	}
}
