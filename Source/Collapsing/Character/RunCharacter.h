// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RunCharacter.generated.h"

UCLASS()
class COLLAPSING_API ARunCharacter final : public ACharacter
{
	GENERATED_BODY()

public:
	bool bCanCharacterTurn;

	bool bIsDead;

	bool bCanChangeSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asseets")
	TObjectPtr<UParticleSystem> DeathParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asseets")
	TObjectPtr<USoundBase> DeathSound;

	ARunCharacter();

	UFUNCTION(BlueprintCallable)
	void Death();

protected:
	UPROPERTY()
	FTimerHandle RestartTimerHandle;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnDeath();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> SecondCamera;

	void SetCameraAndArm();

	void SetCharacterMovement() const;	
};
