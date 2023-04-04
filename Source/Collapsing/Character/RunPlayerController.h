// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "RunPlayerController.generated.h"

UCLASS()
class COLLAPSING_API ARunPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	FRotator DesiredRotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ARunCharacter> RunCharacter;

	void MoveForward(const FRotator& ControlRot);

	void TurnCorner(const FRotator& ControlRot);

public:
	ARunPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void MoveWithoutTurn(const FInputActionValue& Value);

	void ReadyToTurn(const FInputActionValue& Value);

	void Jump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);

	void ChangeSpeed(const FInputActionValue& Value);

	void ChangeView(const FInputActionValue& Value);
	void ResetView(const FInputActionValue& Value);

protected:
	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputDataAsset* InputActions;
};											  
