// Fill out your copyright notice in the Description page of Project Settings.

#include "CCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CWidgetComponent.h"
#include "Stat/CCharacterStatComponent.h"
#include "UI/CHpBarWidget.h"
#include "Game/CollapsingGameMode.h"

ACCharacter::ACCharacter()
{
	bUseControllerRotationYaw = false;
	bCanCharacterTurn = false;
	bIsDead = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 300.f, 0.f);

	CanOpenDoor.Add(EDoorType::Stage, false);
	CanOpenDoor.Add(EDoorType::Quit, false);

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	SetPlayCameraAndArm();
	SetMenuCameraAndArm();
	SetStatAndWidget();

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

void ACCharacter::Tick(float DeltaSeconds)
{
	//GetCharacterMovement()->MaxWalkSpeed = FMath::Clamp(GetCharacterHp() * 20.f, 300.f, 800.f);
}

void ACCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayCamera->Deactivate();
	MenuCamera->Activate();
}

void ACCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
}

void ACCharacter::ApplyDamage(const float InDamage) const
{
	if (IsValid(Stat))
	{
		Stat->ApplyDamage(InDamage);
	}
}

void ACCharacter::EarnHpUpItem()
{
	if (IsValid(Stat))
	{
		Stat->SetHp(Stat->GetCurrentHp() + 20.f);
	}
}

void ACCharacter::HitBySomething(const float LaunchRatio)
{
	FVector LaunchVector = GetActorForwardVector() * -1200.f * LaunchRatio;

	LaunchVector.Z += 250.f * LaunchRatio;

	LaunchCharacter(LaunchVector, true, true);
	ApplyDamage(10.f);
	if (bIsCrouched == true)
	{
		UnCrouch();
	}
}

void ACCharacter::SetCanOpenDoor(const EDoorType InType, const bool InStatus)
{
	CanOpenDoor[InType] = InStatus;
}

EDoorType ACCharacter::GetWhichDoorCanOpen()
{
	for (const auto& CheckDoor : CanOpenDoor)
	{
		if (CheckDoor.Value == true)
		{
			return CheckDoor.Key;
		}
	}
	return EDoorType::None;
}

void ACCharacter::OpenDoor(EDoorType InType)
{
	ICGameModeInterface* GameMode = Cast<ICGameModeInterface>(GetWorld()->GetAuthGameMode());
	if (GameMode != nullptr)
	{
		switch (InType)
		{
		default: break;

		case EDoorType::Stage:
			SetupCharacterMovement();
			GameMode->StartStage();
			SetActorLocationAndRotation({-700.f, 380.f, 100.f}, {0.f, 0.f, 0.f});
			break;

		case EDoorType::Quit:
			GameMode->ExitGame();
			break;
		}
	}
}

float ACCharacter::GetCharacterHp() const
{
	if (IsValid(Stat))
	{
		return Stat->GetCurrentHp();
	}
	return -1.f;
}

float ACCharacter::GetCharacterMaxHp() const
{
	if (IsValid(Stat))
	{
		return Stat->GetMaxHp();
	}
	return -1.f;
}

void ACCharacter::SetPlayCameraAndArm()
{
	PlayCameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("PlayCameraArm"));
	if (!IsValid(PlayCameraArm))
	{
		return;
	}
	PlayCameraArm->SetupAttachment(GetMesh());

	PlayCameraArm->bDoCollisionTest = true;
	PlayCameraArm->bUsePawnControlRotation = true;

	PlayCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayCamera"));
	if (!IsValid(PlayCamera))
	{
		return;
	}
	PlayCamera->bUsePawnControlRotation = false;
	PlayCamera->SetupAttachment(PlayCameraArm, USpringArmComponent::SocketName);
	PlayCamera->SetRelativeRotation(FRotator(-40.f, 0.f, 0.f));
}

void ACCharacter::SetMenuCameraAndArm()
{
	MenuCameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MenuCameraArm"));
	if (!IsValid(MenuCameraArm))
	{
		return;
	}
	MenuCameraArm->SetupAttachment(GetMesh());

	MenuCameraArm->bDoCollisionTest = true;
	MenuCameraArm->bUsePawnControlRotation = true;

	MenuCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MenuCamera"));
	if (!IsValid(MenuCamera))
	{
		return;
	}
	MenuCamera->bUsePawnControlRotation = false;
	MenuCamera->SetupAttachment(MenuCameraArm, USpringArmComponent::SocketName);
	MenuCamera->SetRelativeRotation(FRotator(-40.f, 0.f, 0.f));
}

void ACCharacter::SetupCharacterWidget(UCUserWidget* InUserWidget)
{
	UCHpBarWidget* HpBarWidget = Cast<UCHpBarWidget>(InUserWidget);
	if (IsValid(HpBarWidget))
	{
		HpBarWidget->SetMaxHp(Stat->GetMaxHp());
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UCHpBarWidget::UpdateHpBar);
		Stat->OnHpZero.AddUObject(this, &ACCharacter::Death);
	}
}

void ACCharacter::ChangeStatus()
{
}

void ACCharacter::SetupCharacterMovement() const
{
	UCharacterMovementComponent* CMC = GetCharacterMovement();
	if (IsValid(CMC))
	{
		CMC->MaxWalkSpeedCrouched = 800.f;
		CMC->MaxWalkSpeed = 800.f;
		CMC->JumpZVelocity = 500.f;
		CMC->AirControl = 1.f;
	}
}

void ACCharacter::SetStatAndWidget()
{
	Stat = CreateDefaultSubobject<UCCharacterStatComponent>(TEXT("Stat"));
	HpBar = CreateDefaultSubobject<UCWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh(), TEXT("head"));

	static ConstructorHelpers::FClassFinder<UCUserWidget> HpBarWidgetRef(TEXT("/Game/Collapsing/UI/WBP_HpBar.WBP_HpBar_C"));
	if (IsValid(HpBarWidgetRef.Class))
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.f, 10.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ACCharacter::Death(AActor* CausedActor)
{
	if (bIsDead == false)
	{
		const FVector Location = GetActorLocation();
		const UWorld* CurrentWorld = GetWorld();
		if (IsValid(CurrentWorld))
		{
			bIsDead = true;
			GetController()->DisableInput(nullptr);
			GetController()->SetActorTickEnabled(false);

			if (DeathParticleSystem != nullptr)
			{
				UGameplayStatics::SpawnEmitterAtLocation(CurrentWorld, DeathParticleSystem, Location);
			}
			if (DeathSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(CurrentWorld, DeathSound, Location);
			}
			HpBar->SetVisibility(false);

			if (CausedActor != nullptr)
			{
				GetLocalViewingPlayerController()->SetViewTargetWithBlend(CausedActor, 0.5f);
			}
			DeathDelayTime = 1.5f;
			GetWorldTimerManager().SetTimer(DeathTimerHandler, this, &ACCharacter::RestartGame, DeathDelayTime, false);
		}
	}
}

void ACCharacter::RestartGame()
{
	ICGameModeInterface* GameMode = Cast<ICGameModeInterface>(GetWorld()->GetAuthGameMode());
	if (GameMode != nullptr)
	{
		GameMode->RestartGame();
	}
}
