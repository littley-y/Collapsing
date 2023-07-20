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

// Camera System
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	void SetCameraAndArm();

// Movement System
public:
	virtual void SetTurnStatus(bool InStatus) override;
	virtual bool GetTurnStatus() const override { return bCanCharacterTurn; }

protected:
	bool bCanCharacterTurn;

	void SetupCharacterMovement() const;

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
	virtual void EarnHpUpItem() override;
	
	virtual void HitBySomething(const float LaunchRatio) override;

// Death System
public:
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Particle")
	TObjectPtr<UParticleSystem> DeathParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<USoundBase> DeathSound;
	
	UFUNCTION(BlueprintCallable)
	virtual void Death(const int32 DeathType = 0) override;

protected:
	UPROPERTY()
	FTimerHandle RestartTimerHandle;

	UFUNCTION()
	void OnDeath();

};
