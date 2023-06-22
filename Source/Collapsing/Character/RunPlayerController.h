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

public:
	ARunPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void Move(const FInputActionValue& Value);

	void Turn(const FInputActionValue& Value);

	void Jump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);

	void ChangeSpeed(const FInputActionValue& Value);

	void Slide(const FInputActionValue& Value);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputDataAsset> InputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<class UAnimMontage> SlideMontage;

	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

private:
	bool bControllerCanTurn = false;

	FRotator DesiredRotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ARunCharacter> RunCharacter;

	void TurnController(const FRotator& ControlRot);
};											  
