// Fill out your copyright notice in the Description page of Project Settings.

#include "RunCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

ARunCharacter::ARunCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	bUseControllerRotationYaw = false;
	bCanCharacterTurn = false;
	bIsDead = false;
	bCanChangeSpeed = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	SetCameraAndArm();
	SetCharacterMovement();
}

void ARunCharacter::SetCameraAndArm()
{
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	if (!IsValid(CameraArm))
	{
		return;
	}
	CameraArm->SetupAttachment(GetRootComponent());
	CameraArm->TargetArmLength = 600.f;
	CameraArm->SocketOffset = FVector(-400.f, 0.f, 900.f);

	CameraArm->bDoCollisionTest = false;
	CameraArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if (!IsValid(Camera))
	{
		return;
	}
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	Camera->SetRelativeRotation(FRotator(-40.f, 0.f, 0.f));

	SecondCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Second Camera"));
	if (!IsValid(SecondCamera))
	{
		return;
	}
	SecondCamera->SetupAttachment(GetMesh(), TEXT("head"));
}

void ARunCharacter::BeginPlay()
{
	Super::BeginPlay();

	Collapsed = 0;
	GetWorldTimerManager().SetTimer(CollapsedTimerHandle, this, &ARunCharacter::ChangeCharacterState, 1.f, true);
}

void ARunCharacter::SetCharacterMovement() const
{
	const UCharacterMovementComponent* CMC = GetCharacterMovement();
	if (IsValid(CMC))
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 300.f,0.f);

		GetCharacterMovement()->MaxWalkSpeed = 800.f;
		GetCharacterMovement()->JumpZVelocity = 500.f;
		GetCharacterMovement()->AirControl = 2.f;
	}
}

void ARunCharacter::Death()
{
	if (bIsDead == false)
	{
		const FVector Location = GetActorLocation();

		const UWorld* CurrentWorld = GetWorld();
		if (IsValid(CurrentWorld))
		{
			bIsDead = true;
			GetController()->DisableInput(nullptr);

			if (DeathParticleSystem != nullptr)
			{
				UGameplayStatics::SpawnEmitterAtLocation(CurrentWorld, DeathParticleSystem, Location);
			}
			if (DeathSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(CurrentWorld, DeathSound, Location);
			}
			GetMesh()->SetVisibility(false);

			CurrentWorld->GetTimerManager().SetTimer(RestartTimerHandle, this, &ARunCharacter::OnDeath, 1.f);
		}
	}
}

void ARunCharacter::OnDeath()
{
	bIsDead = false;

	if (RestartTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(RestartTimerHandle);
	}

	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("RestartLevel"));
}

void ARunCharacter::ChangeCharacterState()
{
	++Collapsed;
	GetCharacterMovement()->MaxWalkSpeed -= 40;
	if (Collapsed == 20)
	{
		Death();
	}
}

