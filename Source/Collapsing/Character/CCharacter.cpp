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
#include "Interface/CCharacterControllerInterface.h"

ACCharacter::ACCharacter()
{
	SetActorTickEnabled(false);
	bUseControllerRotationYaw = false;
	bCanCharacterTurn = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 350.f;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 300.f, 0.f);

	CanOpenDoor.Add(EDoorType::Stage, false);
	CanOpenDoor.Add(EDoorType::Arcade, false);
	CanOpenDoor.Add(EDoorType::Quit, false);
	CanOpenDoor.Add(EDoorType::None, false);

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	SetMenuCameraAndArm();
	SetPlayCameraAndArm();
	SetStatAndWidget();
}

void ACCharacter::BeginPlay()
{
	Super::BeginPlay();

	MenuCamera->Activate();
	PlayCamera->Deactivate();
	HpBar->Deactivate();
	HpBar->SetHiddenInGame(true);
}

void ACCharacter::ApplyDamage(const float InDamage) const
{
	if (IsValid(Stat))
	{
		Stat->ApplyDamage(InDamage);
	}
}

void ACCharacter::SetCharacterSpeed(const float CurrHp) const
{
	if (CurrHp < 100.f)
	{
		GetCharacterMovement()->MaxWalkSpeed = FMath::Clamp(CurrHp * 8.f, 200.f, 800.f);
	}
}

void ACCharacter::EarnHpUpItem()
{
	if (IsValid(Stat))
	{
		Stat->SetHp(Stat->GetCurrentHp() + 20.f);
	}
}

void ACCharacter::HitBySomething()
{
	FVector LaunchVector = GetActorForwardVector() * -800.f;

	LaunchVector.Z += 250.f;

	LaunchCharacter(LaunchVector, true, true);
	ApplyDamage(10.f);

	if (bIsCrouched == true)
	{
		UnCrouch();
	}

	if (IsValid(HurtSound))
	{
		UGameplayStatics::SpawnSoundAttached(HurtSound, GetMesh());
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

void ACCharacter::OpenDoor()
{
	ChangeCharacterStatus();
	SetActorLocationAndRotation({ -700.f, 380.f, 100.f }, FRotator::ZeroRotator);
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
		Stat->OnHpChanged.AddUObject(this, &ACCharacter::SetCharacterSpeed);
		Stat->OnHpZero.AddUObject(this, &ACCharacter::Death);
	}
}

void ACCharacter::ChangeCharacterStatus()
{
	PlayCamera->Activate();
	MenuCamera->Deactivate();
	HpBar->SetHiddenInGame(false);

	SetCanTurn(false);

	UCharacterMovementComponent* CMC = GetCharacterMovement();
	if (IsValid(CMC))
	{
		CMC->MaxWalkSpeedCrouched = 800.f;
		CMC->MaxWalkSpeed = 800.f;
		CMC->JumpZVelocity = 500.f;
		CMC->AirControl = 0.5f;
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
	ICCharacterControllerInterface* CharacterController = Cast<ICCharacterControllerInterface>(GetController());
	if (CharacterController != nullptr)
	{
		CharacterController->GameOver();
	}

	if (IsValid(DeathSound))
	{
		UGameplayStatics::SpawnSoundAttached(DeathSound, GetMesh());
	}

	HpBar->Deactivate();
	HpBar->SetHiddenInGame(true);

	if (IsValid(CausedActor))
	{
		GetLocalViewingPlayerController()->SetViewTargetWithBlend(CausedActor, 0.5f);
	}

}
