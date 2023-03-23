// Fill out your copyright notice in the Description page of Project Settings.


#include "RunPlayerController.h"

#include "RunCharacter.h"
#include "Collapsing/Data/InputDataAsset.h"
#include "GameFramework/Character.h"

ARunPlayerController::ARunPlayerController(const FObjectInitializer& ObjectInitializer)
{
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
		                                &ARunPlayerController::Move);
		PlayerEnhancedInput->BindAction(InputActions->InputTurn, ETriggerEvent::Started, this,
		                                &ARunPlayerController::Turn);

		PlayerEnhancedInput->BindAction(InputActions->InputJump, ETriggerEvent::Triggered, this,
		                                &ARunPlayerController::Jump);
		PlayerEnhancedInput->BindAction(InputActions->InputJump, ETriggerEvent::Completed, this,
		                                &ARunPlayerController::StopJump);

		PlayerEnhancedInput->BindAction(InputActions->InputChangeView, ETriggerEvent::Triggered, this,
		                                &ARunPlayerController::ChangeView);
		PlayerEnhancedInput->BindAction(InputActions->InputChangeView, ETriggerEvent::Completed, this,
		                                &ARunPlayerController::ResetView);

	}

}

void ARunPlayerController::Move(const FInputActionValue& Value)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(GetCharacter());
	if (RunCharacter != nullptr && RunCharacter->bCanTurn == false)
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();

		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		RunCharacter->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ARunPlayerController::Turn(const FInputActionValue& Value)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(GetCharacter());
	float Amount = Value.Get<float>();
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

	ARunCharacter* RunCharacter = Cast<ARunCharacter>(GetCharacter());

	if (RunCharacter != nullptr && RunCharacter->bIsDead == false)
	{
		FRotator ForwardRot = this->GetControlRotation();
		ForwardRot.Roll = 0.f;
		ForwardRot.Pitch = 0.f;

		GetCharacter()->AddMovementInput(ForwardRot.Vector());	
	}

}
