// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RunCharacter.generated.h"

UCLASS()
class COLLAPSING_API ARunCharacter final : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* SecondCamera;

	void SetCameraAndArm() const;

	void SetMovement() const;

public:
	
	ARunCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asseets")
	UParticleSystem* DeathParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asseets")
	USoundBase* DeathSound;

	UFUNCTION(BlueprintCallable)
	void Death();

	bool bCanTurn = false;

	bool bIsDead = false;

	bool bCanChangeSpeed = false;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnDeath();

	UPROPERTY()
	FTimerHandle RestartTimerHandle;

public:	
	virtual void Tick(float DeltaTime) override;
};
