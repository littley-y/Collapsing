// Fill out your copyright notice in the Description page of Project Settings.

#include "CPlayerController.h"
#include "CCharacter.h"
#include "Data/CInputDataAsset.h"
#include "GameFramework/Character.h"
#include "Game/CollapsingGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"

ACPlayerController::ACPlayerController(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	bControllerCanTurn = false;
	CurrControllerType = ECharacterControllerType::MainMenu;
}

void ACPlayerController::BeginPlay()
{
	Super::BeginPlay();

	RunCharacter = Cast<ACCharacter>(GetCharacter());

	const FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
	SetCharacterController(ECharacterControllerType::MainMenu);
}

void ACPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearAllTimersForObject(this);
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

	const FRotator CurrRotation = GetControlRotation();
	const FRotator YawRotation = { 0.f, CurrRotation.Yaw, 0.f };

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (IsValid(RunCharacter))
	{
		RunCharacter->AddMovementInput(RightDirection, MovementVector.X);
		if (CurrControllerType == ECharacterControllerType::MainMenu)
		{
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			RunCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
		}
		else if (CurrControllerType == ECharacterControllerType::Play)
		{
			RunCharacter->AddMovementInput(CurrRotation.Vector());
		}
	}
}

void ACPlayerController::Turn(const FInputActionValue& Value)
{
	if (IsValid(RunCharacter) && RunCharacter->GetCanTurn() == true)
	{
		const float TurnAxisFloat = Value.Get<float>();

		const float TurnAngle = 90.f * (CurrControllerType == ECharacterControllerType::MainMenu ? -1 : 1);
		DesiredRotation = GetControlRotation();
		DesiredRotation.Yaw += TurnAxisFloat * TurnAngle;
		bControllerCanTurn = true;
		if (CurrControllerType == ECharacterControllerType::Play)
		{
			RunCharacter->SetCanTurn(false);
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
	if (IsValid(RunCharacter))
	{
		if (RunCharacter->GetCharacterMovement()->IsFalling() != true)
		{
			RunCharacter->Crouch();
		}
	}
}

void ACPlayerController::OpenDoor(const FInputActionValue& Value)
{
	if (IsValid(RunCharacter))
	{
		const EDoorType CurrDoorType = RunCharacter->GetWhichDoorCanOpen();
		ICGameModeInterface* GameMode = Cast<ICGameModeInterface>(GetWorld()->GetAuthGameMode());

		if (GameMode != nullptr)
		{
			switch (CurrDoorType)
			{
			default: break;

			case EDoorType::Stage:
				SetCharacterController(ECharacterControllerType::Play);
				RunCharacter->OpenDoor();
				GameMode->StartGame("Stage");
				break;

			case EDoorType::Arcade:
				SetCharacterController(ECharacterControllerType::Play);
				RunCharacter->OpenDoor();
				GameMode->StartGame("Arcade");
				break;

			case EDoorType::Quit:
				GameMode->ExitGame();
				break;
			}
		}
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

void ACPlayerController::GameOver()
{
	ICGameModeInterface* GameMode = Cast<ICGameModeInterface>(GetWorld()->GetAuthGameMode());
	if (GameMode != nullptr)
	{
		DisableInput(nullptr);
		SetActorTickEnabled(false);

		GameMode->SetTimer(1);

		DeathDelayTime = 2.f;
		GetWorldTimerManager().SetTimer(DeathTimerHandler, this, &ACPlayerController::RestartLevel, DeathDelayTime,
		                                false);
	}
}

void ACPlayerController::SetCharacterController(const ECharacterControllerType InControllerType)
{
	CurrControllerType = InControllerType;

	if (CurrControllerType == ECharacterControllerType::Play)
	{
		SetControlRotation({ 0.f, 0.f, 0.f });
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	if (IsValid(Subsystem))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputMappings[CurrControllerType], 0);
	}
}
