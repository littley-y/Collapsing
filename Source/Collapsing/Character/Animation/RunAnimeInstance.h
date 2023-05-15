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
	URunAnimeInstance();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	float GroundSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	float MovingThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	float JumpingThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	uint8 bIsFalling : 1;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterMovementComponent> Movement;


};
