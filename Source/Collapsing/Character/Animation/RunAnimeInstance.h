// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RunAnimeInstance.generated.h"

UCLASS()
class COLLAPSING_API URunAnimeInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Value")
	float Speed;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Value")
	bool bIsInAir;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Pawn")
	APawn* Pawn;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess))
	TObjectPtr<class ARunCharacter> RunCharacter;

};
