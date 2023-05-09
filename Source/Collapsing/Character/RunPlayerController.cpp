﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "RunPlayerController.h"
#include "RunCharacter.h"
#include "Data/InputDataAsset.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

ARunPlayerController::ARunPlayerController(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	DesiredRotation = AController::GetControlRotation();
}

void ARunPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!IsValid(Subsystem))
	{
		return;
	}
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* PlayerEnhancedInput = CastChecked<UEnhancedInputComponent>(InputComponent);
	PlayerEnhancedInput->BindAction(InputActions->MoveAction, ETriggerEvent::Triggered, this,
									&ARunPlayerController::Move);
	PlayerEnhancedInput->BindAction(InputActions->ChangeSpeedAction, ETriggerEvent::Started, this,
		                            &ARunPlayerController::ChangeSpeed);
	PlayerEnhancedInput->BindAction(InputActions->TurnAction, ETriggerEvent::Started, this,
		                            &ARunPlayerController::Turn);
	PlayerEnhancedInput->BindAction(InputActions->JumpAction, ETriggerEvent::Started, this,
		                            &ARunPlayerController::Jump);
	PlayerEnhancedInput->BindAction(InputActions->JumpAction, ETriggerEvent::Completed, this,
		                            &ARunPlayerController::StopJump);
}

void ARunPlayerController::Move(const FInputActionValue& Value)
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

void ARunPlayerController::Turn(const FInputActionValue& Value)
{
	if (IsValid(RunCharacter) && RunCharacter->bCanCharacterTurn == true)
	{
		const float TurnAxisFloat = Value.Get<float>();
		DesiredRotation = GetControlRotation();
		DesiredRotation.Yaw += TurnAxisFloat * 90;
		RunCharacter->bCanCharacterTurn = false;
		bControllerCanTurn = true;

		UE_LOG(LogTemp, Warning, TEXT("Set DesiredRotation : %s"), *DesiredRotation.ToString());
	}
}

void ARunPlayerController::Jump(const FInputActionValue& Value)
{
	if (IsValid(RunCharacter))
	{
		RunCharacter->Jump();
		UE_LOG(LogTemp, Warning, TEXT("Jump Start"));
	}
}

void ARunPlayerController::StopJump(const FInputActionValue& Value)
{
	if (IsValid(RunCharacter))
	{
		RunCharacter->StopJumping();
		UE_LOG(LogTemp, Warning, TEXT("Jump End"));
	}
}

void ARunPlayerController::ChangeSpeed(const FInputActionValue& Value)
{
	if (IsValid(RunCharacter) && RunCharacter->bCanChangeSpeed == true)
	{
		float& CharacterSpeed = RunCharacter->GetCharacterMovement()->MaxWalkSpeed;
		if (CharacterSpeed > 1600.f)
		{
			return;
		}

		const float TargetSpeed = Value.Get<float>() * 200.f;
		CharacterSpeed += TargetSpeed;

		if (CharacterSpeed > 0.f)
		{
			RunCharacter->bCanChangeSpeed = false;
			UE_LOG(LogTemp, Warning, TEXT("현재 속도 %f"), CharacterSpeed)
		}
		else
		{
			RunCharacter->Death();
		}
	}
}

void ARunPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsValid(RunCharacter) && RunCharacter->bIsDead == false)
	{
		const FRotator ControlRot = GetControlRotation();
		RunCharacter->AddMovementInput(ControlRot.Vector()); // 캐릭터가 자동으로 앞으로 이동하도록

		if (bControllerCanTurn == true)
		{
			TurnController(ControlRot);
		}
	}
}

void ARunPlayerController::TurnController(const FRotator& ControlRot)
{
	const FRotator InterpolatedRot = FMath::RInterpTo(ControlRot, DesiredRotation, GetWorld()->GetDeltaSeconds(), 20.f);
	SetControlRotation(InterpolatedRot);

	if (DesiredRotation.Equals(InterpolatedRot, 0.01f))
	{
		bControllerCanTurn = false;
		UE_LOG(LogTemp, Warning, TEXT("Rotation finished"));
	}
}

void ARunPlayerController::BeginPlay()
{
	Super::BeginPlay();

	RunCharacter = Cast<ARunCharacter>(GetCharacter());
	const FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
