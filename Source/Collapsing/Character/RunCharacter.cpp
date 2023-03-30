// Fill out your copyright notice in the Description page of Project Settings.


#include "RunCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARunCharacter::ARunCharacter()
{
	bUseControllerRotationYaw = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 300.f,0.f);

	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.1f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	// Create a camera arm
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(GetRootComponent());
	CameraArm->TargetArmLength = 600.f;
	CameraArm->SocketOffset = FVector(-400.f, 0.f, 900.f);

	CameraArm->bDoCollisionTest = false;
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->bEnableCameraLag = true;
	CameraArm->bEnableCameraRotationLag = true;
	CameraArm->CameraLagSpeed = 8.f;
	CameraArm->CameraRotationLagSpeed = 8.f;

	// Create default camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	Camera->SetRelativeRotation(FRotator(-40.f, 0.f, 0.f));

	SecondCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Second Camera"));
	SecondCamera->SetupAttachment(GetMesh(), TEXT("head"));

}

// Called when the game starts or when spawned
void ARunCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float Speed = FVector::DotProduct(GetVelocity(), GetActorRotation().Vector());
	if (FMath::IsNearlyEqual(Speed, 0.f, 0.1f))
	{
		Death();
	}
}

void ARunCharacter::Death()
{
	if (bIsDead == false)
	{
		const FVector Location = GetActorLocation();

		const UWorld* World = GetWorld();
		if (World != nullptr)
		{
			bIsDead = true;
			GetController()->DisableInput(nullptr);

			if (DeathParticleSystem != nullptr)
			{
				UGameplayStatics::SpawnEmitterAtLocation(World, DeathParticleSystem, Location);
			}
			if (DeathSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(World, DeathSound, Location);
			}
			GetMesh()->SetVisibility(false);

			World->GetTimerManager().SetTimer(RestartTimerHandle, this, &ARunCharacter::OnDeath, 1.f);
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

