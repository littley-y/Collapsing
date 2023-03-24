// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "RunPlayerController.generated.h"

/**
 * 
 */

UCLASS()
class COLLAPSING_API ARunPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARunPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void Move(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);

	void Jump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);

	void ChangeView(const FInputActionValue& Value);
	void ResetView(const FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Charactor")
	class ARunCharacter* RunCharacter;

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

protected:
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputDataAsset* InputActions;
};											  
