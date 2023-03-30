// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RunAnimeInstance.generated.h"

/**
 * 
 */
UCLASS()
class COLLAPSING_API URunAnimeInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Movement")
	float Speed;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="MoveMent")
	bool bIsInAir;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="MoveMent")
	APawn* Pawn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="MoveMent")
	TObjectPtr<UPawnMovementComponent> MovementComponent;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

};
