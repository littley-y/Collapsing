// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CControlDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class COLLAPSING_API UCControlDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	float TurnAngle;

	UPROPERTY()
	float WalkSpeed;

	UPROPERTY()
	uint8 bOrientRotationToMovement : 1; 

	UPROPERTY()
	float AirControl;

	UPROPERTY()
	float JumpZVelocity;
};
