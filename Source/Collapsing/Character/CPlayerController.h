// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

UENUM()
enum class ECharacterControllerType : uint8
{
	MainMenu,
	Play
};

UCLASS()
class COLLAPSING_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void Move(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);
	void Slide(const FInputActionValue& Value);
	void OpenDoor(const FInputActionValue& Value);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TMap<ECharacterControllerType, TObjectPtr<UInputMappingContext>> InputMappings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UCInputDataAsset> InputActions;

	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	void SetCharacterControl(ECharacterControllerType InControllerType) const;

private:
	bool bControllerCanTurn;
	FRotator DesiredRotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ACCharacter> RunCharacter;

	void TurnController(const FRotator& ControlRot);

	ECharacterControllerType CurrControllerType;
};											  
