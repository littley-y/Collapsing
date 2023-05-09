// Fill out your copyright notice in the Description page of Project Settings.

#include "RunCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Game/CollapsingGameInstance.h"

ARunCharacter::ARunCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
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

	UWorld* CurrWorld = GetWorld();
	if (IsValid(CurrWorld))
	{
		CurrWorld->GetTimerManager().SetTimer(CheckCollapsedTimerHandle, this, &ARunCharacter::CheckCollapsed, 1.f, true);
	}
}

void ARunCharacter::CheckCollapsed()
{
	UCollapsingGameInstance* CGI = Cast<UCollapsingGameInstance>(GetGameInstance());
	if (IsValid(CGI))
	{
		const int IntCollapsed = FMath::RoundToInt(CGI->GetCollapsed());
		const float TargetSpeed = (IntCollapsed / 5 + 1) * 200.f;
		GetCharacterMovement()->MaxWalkSpeed = TargetSpeed;
		UE_LOG(LogTemp, Warning, TEXT("Current Speed : %f"), TargetSpeed)
	}
}

void ARunCharacter::SetCharacterMovement() const
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 300.f,0.f);

	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 2.f;
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
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

