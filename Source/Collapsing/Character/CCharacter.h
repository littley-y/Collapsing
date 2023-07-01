// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/CCharacterWidgetInterface.h"
#include "CCharacter.generated.h"


UCLASS()
class COLLAPSING_API ACCharacter : public ACharacter, public ICCharacterWidgetInterface
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
	bool bCanCharacterTurn;

	bool bCanChangeSpeed;

protected:
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

// Item System
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<USoundBase> PickSound;

	void EarnHpUpItem() const;

// Animation System
public:
	virtual void Crouch(bool bClientSimulation) override;
	void OnMontageEnded(UAnimMontage* Montage, bool Interrupted);

	UFUNCTION(BlueprintCallable)
	void HitByWall();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<class UAnimMontage> SlideMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<class UAnimMontage> FallingBackMontage;

// Death System
public:
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Particle")
	TObjectPtr<UParticleSystem> DeathParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<USoundBase> DeathSound;
	
	UFUNCTION(BlueprintCallable)
	void Death();

protected:
	UPROPERTY()
	FTimerHandle RestartTimerHandle;

	UFUNCTION()
	void OnDeath();

};
