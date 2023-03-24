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
		                                &ARunPlayerController::SetDesiredRotation);

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
	if (RunCharacter != nullptr)
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();

		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		RunCharacter->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ARunPlayerController::SetDesiredRotation(const FInputActionValue& Value)
{
	if (RunCharacter != nullptr)
	{
		const float TurnAxisFloat = Value.Get<float>();

		DesiredRotation = GetControlRotation();
		DesiredRotation.Yaw += TurnAxisFloat * 90;
		RunCharacter->bCanTurn = false;
	}
}

void ARunPlayerController::Jump(const FInputActionValue& Value)
{
	GetCharacter()->Jump();
	UE_LOG(LogTemp, Warning, TEXT("점프키 눌림"));
}

void ARunPlayerController::StopJump(const FInputActionValue& Value)
{
	GetCharacter()->StopJumping();
	UE_LOG(LogTemp, Warning, TEXT("점프키 떼짐"));

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

	if (RunCharacter != nullptr && RunCharacter->bIsDead == false)
	{
		const FRotator ControlRot = this->GetControlRotation();

		GetCharacter()->AddMovementInput(ControlRot.Vector());

		// 컨트롤러를 매 tick마다 90도 혹은 -90도로 보간하여 회전시킵니다.
		if (RunCharacter->bCanTurn == true && ControlRot != DesiredRotation)
		{
			SetControlRotation(FMath::RInterpTo(ControlRot, DesiredRotation, DeltaSeconds, 10.f));
		}
	}

}

void ARunPlayerController::BeginPlay()
{
	Super::BeginPlay();

	RunCharacter = Cast<ARunCharacter>(GetCharacter());
}
