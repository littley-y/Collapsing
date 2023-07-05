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

ACCharacter::ACCharacter()
{
	bUseControllerRotationYaw = false;
	bCanCharacterTurn = false;
	bIsDead = false;
	bCanChangeSpeed = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	SetCameraAndArm();
	SetupCharacterMovement();
	SetStatAndWidget();

	FallingBackMontage = CreateDefaultSubobject<UAnimMontage>(TEXT("FallingBackMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FallingBackMontageRef(
		TEXT("/Script/Engine.AnimMontage'/Game/Collapsing/Character/Animation/AM_FallingBack.AM_FallingBack'"));
	if (IsValid(FallingBackMontageRef.Object))
	{
		FallingBackMontage = FallingBackMontageRef.Object;
	}
}

void ACCharacter::Tick(float DeltaSeconds)
{
	GetCharacterMovement()->MaxWalkSpeed = FMath::Clamp(GetCharacterHp() * 20.f, 400.f, 1000.f);
}

void ACCharacter::EarnHpUpItem() const
{
	if (IsValid(Stat))
	{
		Stat->SetHp(Stat->GetCurrentHp() + 20.f);

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

void ACCharacter::SetCameraAndArm()
{
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	if (!IsValid(CameraArm))
	{
		return;
	}
	CameraArm->SetupAttachment(GetMesh());

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

void ACCharacter::SetupCharacterMovement() const
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

void ACCharacter::HitByWall()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(FallingBackMontage, 1.f);

	FOnMontageEnded FallingBackEndDelegate;
	FallingBackEndDelegate.BindUObject(this, &ACCharacter::OnHitByWallEnded);
	AnimInstance->Montage_SetEndDelegate(FallingBackEndDelegate, FallingBackMontage);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController))
	{
		PlayerController->DisableInput(PlayerController);
	}
}

void ACCharacter::OnHitByWallEnded(UAnimMontage* Montage, bool Interrupted)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController))
	{
		PlayerController->EnableInput(PlayerController);
	}
}

void ACCharacter::Death()
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

			CurrentWorld->GetTimerManager().SetTimer(RestartTimerHandle, this, &ACCharacter::OnDeath, 1.f);
		}
	}
}

void ACCharacter::OnDeath()
{
	bIsDead = false;

	if (RestartTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(RestartTimerHandle);
	}

	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("RestartLevel"));
}
