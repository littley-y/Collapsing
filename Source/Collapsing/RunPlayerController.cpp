// Fill out your copyright notice in the Description page of Project Settings.


#include "RunPlayerController.h"
#include "InputDataAsset.h"
#include "GameFramework/Character.h"

ARunPlayerController::ARunPlayerController(const FObjectInitializer& ObjectInitializer)
{
}

void ARunPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	Subsystem->AddMappingContext(InputMapping, 0);

	if (UEnhancedInputComponent* PlayerEnhancedInput = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		PlayerEnhancedInput->BindAction(InputActions->InputMove, ETriggerEvent::Triggered, this, &ARunPlayerController::Move);
		PlayerEnhancedInput->BindAction(InputActions->InputJump, ETriggerEvent::Triggered, this, &ARunPlayerController::Jump);
		PlayerEnhancedInput->BindAction(InputActions->InputJump, ETriggerEvent::Completed, this, &ARunPlayerController::StopJump);

		PlayerEnhancedInput->BindAction(InputActions->InputChangeView, ETriggerEvent::Triggered, this, &ARunPlayerController::ChangeView);
		PlayerEnhancedInput->BindAction(InputActions->InputChangeView, ETriggerEvent::Completed, this, &ARunPlayerController::ResetView);
	}

}

void ARunPlayerController::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement
	GetCharacter()->AddMovementInput(RightDirection, MovementVector.X);
}

void ARunPlayerController::Jump(const FInputActionValue& Value)
{
	GetCharacter()->Jump();
	UE_LOG(LogTemp, Warning, TEXT("Á¡ÇÁÅ° ´­¸²"));
}

void ARunPlayerController::StopJump(const FInputActionValue& Value)
{
	GetCharacter()->StopJumping();
	UE_LOG(LogTemp, Warning, TEXT("Á¡ÇÁÅ° ¶¼Áü"));

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

	FRotator ForwardRot = this->GetControlRotation();
	ForwardRot.Roll = 0.f;
	ForwardRot.Pitch = 0.f;

	GetCharacter()->AddMovementInput(ForwardRot.Vector());
}
