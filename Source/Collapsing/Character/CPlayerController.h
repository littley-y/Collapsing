// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Interface/CCharacterControllerInterface.h"
#include "CPlayerController.generated.h"

UENUM()
enum class ECharacterControllerType : uint8
{
	MainMenu,
	Play
};

UCLASS()
class COLLAPSING_API ACPlayerController : public APlayerController, public ICCharacterControllerInterface
{
	GENERATED_BODY()

public:
	ACPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupInputComponent() override;

	// Input Section
public:
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

	void SetCharacterController(ECharacterControllerType InControllerType);
	void TurnController(const FRotator& ControlRot);

private:
	bool bControllerCanTurn;
	FRotator DesiredRotation;
	ECharacterControllerType CurrControllerType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ACCharacter> RunCharacter;

	// GameMode Section
public:
	virtual void GameOver() override;

protected:
	FTimerHandle DeathTimerHandler;

private:
	float DeathDelayTime;


	// UI Section
};				  
