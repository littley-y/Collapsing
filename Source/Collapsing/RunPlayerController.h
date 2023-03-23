// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RunPlayerController.generated.h"

/**
 * 
 */

class UInputMappingContext;
class UInputDataAsset;

UCLASS()
class COLLAPSING_API ARunPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARunPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void Move(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);
	void ChangeView(const FInputActionValue& Value);
	void ResetView(const FInputActionValue& Value);

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputDataAsset* InputActions;
};											  
