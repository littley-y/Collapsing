// Fill out your copyright notice in the Description page of Project Settings.

#include "RunCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CWidgetComponent.h"
#include "Stat/CCharacterStatComponent.h"
#include "UI/CHpBarWidget.h"
#include "Utils/AssetFinder.hpp"

ARunCharacter::ARunCharacter()
{
	bUseControllerRotationYaw = false;
	bCanCharacterTurn = false;
	bIsDead = false;
	bCanChangeSpeed = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	SetCameraAndArm();
	SetupCharacterMovement();
	SetStatAndWidget();
}

void ARunCharacter::EarnHpUpItem() const
{
	if (IsValid(Stat))
	{
		Stat->SetHp(Stat->GetCurrentHp() + 20.f);
		if (IsValid(PickSound))
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickSound, GetActorLocation());
		}
	}
}

float ARunCharacter::GetCharacterHp() const
{
	if (IsValid(Stat))
	{
		return Stat->GetCurrentHp();
	}
	return -1.f;
}

float ARunCharacter::GetCharacterMaxHp() const
{
	if (IsValid(Stat))
	{
		return Stat->GetMaxHp();
	}
	return -1.f;
}

void ARunCharacter::SetCameraAndArm()
{
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	if (!IsValid(CameraArm))
	{
		return;
	}
	CameraArm->SetupAttachment(GetRootComponent());

	CameraArm->bDoCollisionTest = true;
	CameraArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if (!IsValid(Camera))
	{
		return;
	}
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	Camera->SetRelativeRotation(FRotator(-40.f, 0.f, 0.f));
}

void ARunCharacter::SetupCharacterWidget(UCUserWidget* InUserWidget)
{
	UCHpBarWidget* HpBarWidget = Cast<UCHpBarWidget>(InUserWidget);
	if (IsValid(HpBarWidget))
	{
		HpBarWidget->SetMaxHp(Stat->GetMaxHp());
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UCHpBarWidget::UpdateHpBar);
		Stat->OnHpZero.AddUObject(this, &ARunCharacter::Death);
	}
}

void ARunCharacter::SetupCharacterMovement() const
{
	UCharacterMovementComponent* CMC = GetCharacterMovement();
	if (IsValid(CMC))
	{
		CMC->MaxWalkSpeedCrouched = 1000.f;
		CMC->bOrientRotationToMovement = true;
		CMC->RotationRate = FRotator(0.f, 300.f, 0.f);

		CMC->MaxWalkSpeed = 1000.f;
		CMC->JumpZVelocity = 500.f;
		CMC->AirControl = 2.f;
	}
}

void ARunCharacter::Crouch(bool bClientSimulation)
{
}

void ARunCharacter::UnCrouch(bool bClientSimulation)
{
}

void ARunCharacter::SetStatAndWidget()
{
	Stat = CreateDefaultSubobject<UCCharacterStatComponent>(TEXT("Stat"));

	HpBar = CreateDefaultSubobject<UCWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh(), TEXT("head"));
	const TSubclassOf<UCUserWidget> HpBarWidget = MyFunction::AssetClassFinder<UCUserWidget>(
		TEXT("/Game/Collapsing/UI/WBP_HpBar.WBP_HpBar_C"));
	if (IsValid(HpBarWidget))
	{
		HpBar->SetWidgetClass(HpBarWidget);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.f, 10.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
			HpBar->SetVisibility(false);

			CurrentWorld->GetTimerManager().SetTimer(RestartTimerHandle, this, &ARunCharacter::OnDeath, 1.f);
		}
	}
}

void ARunCharacter::Tick(float DeltaSeconds)
{
	GetCharacterMovement()->MaxWalkSpeed = FMath::Clamp(GetCharacterHp() * 20.f, 400.f, 1000.f);
}

void ARunCharacter::ChangeCapsuleSize(float InWidth, float InHeight)
{
	GetCapsuleComponent()->SetCapsuleSize(InWidth, InHeight);
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
