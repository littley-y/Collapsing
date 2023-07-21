﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "CPlayerController.h"
#include "CCharacter.h"
#include "Data/CInputDataAsset.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

ACPlayerController::ACPlayerController(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	bControllerCanTurn = false;

	CurrControllerType = ECharacterControllerType::MainMenu;
}

void ACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* PlayerEnhancedInput = CastChecked<UEnhancedInputComponent>(InputComponent);

	PlayerEnhancedInput->BindAction(InputActions->MoveAction, ETriggerEvent::Triggered, this,
		&ACPlayerController::Move);
	PlayerEnhancedInput->BindAction(InputActions->TurnAction, ETriggerEvent::Started, this,
		&ACPlayerController::Turn);
	PlayerEnhancedInput->BindAction(InputActions->JumpAction, ETriggerEvent::Started, this,
		&ACPlayerController::Jump);
	PlayerEnhancedInput->BindAction(InputActions->JumpAction, ETriggerEvent::Completed, this,
		&ACPlayerController::StopJump);
	PlayerEnhancedInput->BindAction(InputActions->SlideAction, ETriggerEvent::Started, this,
		&ACPlayerController::Slide);
	PlayerEnhancedInput->BindAction(InputActions->OpenDoorAction, ETriggerEvent::Started, this,
		&ACPlayerController::OpenDoor);
}

void ACPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (IsValid(RunCharacter))
	{
		RunCharacter->AddMovementInput(RightDirection, MovementVector.X);
		RunCharacter->AddMovementInput(GetControlRotation().Vector());
	}
}

void ACPlayerController::Turn(const FInputActionValue& Value)
{
	if (IsValid(RunCharacter) && RunCharacter->GetTurnStatus() == true)
	{
		const float TurnAxisFloat = Value.Get<float>();
		DesiredRotation = GetControlRotation();
		DesiredRotation.Yaw += TurnAxisFloat * 90;
		bControllerCanTurn = true;
		if (CurrControllerType == ECharacterControllerType::Play)
		{
			RunCharacter->SetTurnStatus(false);
		}

		UE_LOG(LogTemp, Warning, TEXT("Set DesiredRotation : %s"), *DesiredRotation.ToString());
	}
}

void ACPlayerController::Jump(const FInputActionValue& Value)
{
	if (IsValid(RunCharacter))
	{
		RunCharacter->Jump();
	}
}

void ACPlayerController::StopJump(const FInputActionValue& Value)
{
	if (IsValid(RunCharacter))
	{
		RunCharacter->StopJumping();
	}
}

void ACPlayerController::Slide(const FInputActionValue& Value)
{
	if (IsValid(RunCharacter) && RunCharacter->GetCharacterMovement()->IsFalling() == false)
	{
		DisableInput(this);
		RunCharacter->Crouch();
	}
}

void ACPlayerController::OpenDoor(const FInputActionValue& Value)
{
	if (IsValid(RunCharacter))
	{
		// RunCharacter->OpenDoor();
	}
}


void ACPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsValid(RunCharacter))
	{
		const FRotator ControlRot = GetControlRotation();

		if (CurrControllerType == ECharacterControllerType::Play)
		{
			RunCharacter->AddMovementInput(ControlRot.Vector());
		}

		if (bControllerCanTurn == true)
		{
			TurnController(ControlRot);
		}
	}
}

void ACPlayerController::TurnController(const FRotator& ControlRot)
{
	const FRotator InterpolatedRot = FMath::RInterpTo(ControlRot, DesiredRotation, GetWorld()->GetDeltaSeconds(), 20.f);
	SetControlRotation(InterpolatedRot);

	if (DesiredRotation.Equals(InterpolatedRot, 0.01f))
	{
		bControllerCanTurn = false;
		UE_LOG(LogTemp, Warning, TEXT("Rotation finished"));
	}
}

void ACPlayerController::BeginPlay()
{
	Super::BeginPlay();

	RunCharacter = Cast<ACCharacter>(GetCharacter());
	ensure(RunCharacter);

	const FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
	SetCharacterControl(ECharacterControllerType::MainMenu);
}

void ACPlayerController::SetCharacterControl(const ECharacterControllerType InControllerType) const
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	if (IsValid(Subsystem))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputMappings[InControllerType], 0);
	}
}
