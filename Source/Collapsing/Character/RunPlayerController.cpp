// Fill out your copyright notice in the Description page of Project Settings.

#include "RunPlayerController.h"
#include "RunCharacter.h"
#include "Collapsing/Data/InputDataAsset.h"
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

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());

	Subsystem->AddMappingContext(InputMapping, 0);

	if (UEnhancedInputComponent* PlayerEnhancedInput = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		PlayerEnhancedInput->BindAction(InputActions->InputMove, ETriggerEvent::Triggered, this,
										&ARunPlayerController::MoveWithoutTurn);
		PlayerEnhancedInput->BindAction(InputActions->InputSpeed, ETriggerEvent::Started, this,
		                                &ARunPlayerController::ChangeSpeed);
		PlayerEnhancedInput->BindAction(InputActions->InputTurn, ETriggerEvent::Started, this,
		                                &ARunPlayerController::ReadyToTurn);

		PlayerEnhancedInput->BindAction(InputActions->InputJump, ETriggerEvent::Started, this,
		                                &ARunPlayerController::Jump);
		PlayerEnhancedInput->BindAction(InputActions->InputJump, ETriggerEvent::Completed, this,
		                                &ARunPlayerController::StopJump);

		PlayerEnhancedInput->BindAction(InputActions->InputChangeView, ETriggerEvent::Triggered, this,
		                                &ARunPlayerController::ChangeView);
		PlayerEnhancedInput->BindAction(InputActions->InputChangeView, ETriggerEvent::Completed, this,
		                                &ARunPlayerController::ResetView);
	}
}

void ARunPlayerController::MoveWithoutTurn(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	RunCharacter->AddMovementInput(RightDirection, MovementVector.X);
	RunCharacter->AddMovementInput(GetControlRotation().Vector());
}

void ARunPlayerController::ReadyToTurn(const FInputActionValue& Value)
{
	if (RunCharacter->bCanTurn == true)
	{
		const float TurnAxisFloat = Value.Get<float>();
		DesiredRotation = GetControlRotation();
		DesiredRotation.Yaw += TurnAxisFloat * 90;
		UE_LOG(LogTemp, Warning, TEXT("Set DesiredRotation : %s"), *DesiredRotation.ToString());
	}
}

void ARunPlayerController::Jump(const FInputActionValue& Value)
{
	RunCharacter->Jump();
	UE_LOG(LogTemp, Warning, TEXT("Jump Start"));
}

void ARunPlayerController::StopJump(const FInputActionValue& Value)
{
	RunCharacter->StopJumping();
	UE_LOG(LogTemp, Warning, TEXT("Jump End"));
}

void ARunPlayerController::ChangeSpeed(const FInputActionValue& Value)
{
	if (RunCharacter->bCanChangeSpeed == true)
	{
		const float TargetSpeed = Value.Get<float>() * 200.f;
		RunCharacter->GetCharacterMovement()->MaxWalkSpeed += TargetSpeed;
		UE_LOG(LogTemp, Warning, TEXT("현재 속도 %f"), RunCharacter->GetCharacterMovement()->MaxWalkSpeed)
		RunCharacter->bCanChangeSpeed = false;
	}
}

void ARunPlayerController::ChangeView(const FInputActionValue& Value)
{
}

void ARunPlayerController::ResetView(const FInputActionValue& Value)
{
}

void ARunPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (RunCharacter->bIsDead == false)
	{
		const FRotator ControlRot = GetControlRotation();
		MoveForward(ControlRot);
		TurnCorner(ControlRot);
	}
}

void ARunPlayerController::MoveForward(const FRotator& ControlRot)
{
	RunCharacter->AddMovementInput(ControlRot.Vector());
}

void ARunPlayerController::TurnCorner(const FRotator& ControlRot) // 회전 보간 및 입력 막기.
{
	if (RunCharacter->bCanTurn == true && DesiredRotation.Equals(ControlRot, 0.1f) == false)
	{
		bIsTurning = true;
		RunCharacter->bCanTurn = false;
	}
	if (bIsTurning == true)
	{
		const FRotator InterpolatedRot = FMath::RInterpTo(ControlRot, DesiredRotation, GetWorld()->GetDeltaSeconds(),
		                                                  15.f);
		SetControlRotation(InterpolatedRot);
		if (DesiredRotation.Equals(InterpolatedRot, 0.1f))
		{
			bIsTurning = false;
			UE_LOG(LogTemp, Warning, TEXT("Rotation finished"));
		}
	}
}

void ARunPlayerController::BeginPlay()
{
	Super::BeginPlay();

	RunCharacter = Cast<ARunCharacter>(GetCharacter());
	check(RunCharacter != nullptr)
}
