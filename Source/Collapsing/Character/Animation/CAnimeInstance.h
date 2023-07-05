// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimeInstance.generated.h"

UCLASS()
class COLLAPSING_API UCAnimeInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UCAnimeInstance();

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	uint8 bIsCrouching : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	float CharacterHp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	float CharacterMaxHp;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ACCharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterMovementComponent> Movement;

// Animation Notify
public:
	UFUNCTION()
	void AnimNotify_SlideEnd();

};
