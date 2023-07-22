// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/CCharacterWidgetInterface.h"
#include "Interface/CCharacterInteractionInterface.h"
#include "CCharacter.generated.h"

UCLASS()
class COLLAPSING_API ACCharacter : public ACharacter, public ICCharacterWidgetInterface, public ICCharacterInteractionInterface
{
	GENERATED_BODY()

public:
	ACCharacter();

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

// Camera System
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> PlayCameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> PlayCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> MenuCameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> MenuCamera;

private:
	void SetPlayCameraAndArm();
	void SetMenuCameraAndArm();

// Movement System
public:
	FORCEINLINE virtual void SetCanTurn(const bool InStatus) override { bCanCharacterTurn = InStatus; };
	FORCEINLINE virtual bool GetCanTurn() const override { return bCanCharacterTurn; }

	void ChangeStatus();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, Meta = (AllowPrivatAcces = "true"))
	bool bCanCharacterTurn;

	void SetupCharacterMovement() const;

// Interaction System
public:
	virtual void EarnHpUpItem() override;
	virtual void HitBySomething(const float LaunchRatio) override;
	virtual void SetCanOpenDoor(EDoorType InType, const bool InStatus) override;;
	virtual EDoorType GetWhichDoorCanOpen() override;;

	void OpenDoor(EDoorType InType);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, Meta = (AllowPrivatAcces = "true"))
	TMap<EDoorType, bool> CanOpenDoor;

// Stat System
public:
	UFUNCTION(BlueprintCallable)
	float GetCharacterHp() const;

	UFUNCTION(BlueprintCallable)
	float GetCharacterMaxHp() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivatAcces = "true"))
	TObjectPtr<class UCCharacterStatComponent> Stat;

// UI System
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivatAcces = "true"))
	TObjectPtr<class UCWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(class UCUserWidget* InUserWidget) override;
	void SetStatAndWidget();

// Stat System
public:
	void ApplyDamage(const float InDamage) const;

// Death System
public:
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Particle")
	TObjectPtr<UParticleSystem> DeathParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<USoundBase> DeathSound;
	
	UFUNCTION(BlueprintCallable)
	virtual void Death(AActor* CausedActor) override;

protected:
	FTimerHandle DeathTimerHandler;
	float DeathDelayTime;

	void RestartGame();
};
