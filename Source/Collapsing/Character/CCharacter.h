// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/CCharacterWidgetInterface.h"
#include "Interface/CCharacterInteractionInterface.h"
#include "CCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDoorChangedDelegate, bool)

UCLASS()
class COLLAPSING_API ACCharacter : public ACharacter, public ICCharacterWidgetInterface,
                                   public ICCharacterInteractionInterface
{
	GENERATED_BODY()

public:
	ACCharacter();

protected:
	virtual void BeginPlay() override;

// Camera Section
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

// Movement Section
public:
	FORCEINLINE virtual void SetCanTurn(const bool InStatus) override { bCanCharacterTurn = InStatus; };
	FORCEINLINE virtual bool GetCanTurn() const override { return bCanCharacterTurn; }
	void SetCharacterSpeed(float CurrHp) const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, Meta = (AllowPrivatAcces = "true"))
	bool bCanCharacterTurn;

// Interaction Section
public:
	virtual void EarnHpUpItem() override;
	virtual void HitBySomething() override;
	virtual void ChangeCanDoorOpen(EDoorType InType) override;;
	virtual EDoorType GetWhichDoorCanOpen() override;;

	void ChangeCharacterStatus();
	void OpenDoor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, Meta = (AllowPrivatAcces = "true"))
	TMap<EDoorType, bool> CanOpenDoor;

	FOnDoorChangedDelegate OnDoorChanged;

// Stat Section
public:
	UFUNCTION(BlueprintCallable)
	float GetCharacterHp() const;

	UFUNCTION(BlueprintCallable)
	float GetCharacterMaxHp() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivatAcces = "true"))
	TObjectPtr<class UCCharacterStatComponent> Stat;

// UI Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivatAcces = "true"))
	TObjectPtr<class UCWidgetComponent> HpBarComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivatAcces = "true"))
	TObjectPtr<class UCWidgetComponent> TutorialComponent;

	virtual void SetupCharacterWidget(class UCUserWidget* InUserWidget) override;

	void SetStatAndWidget();

// Stat Section
public:
	void ApplyDamage(const float InDamage) const;

// Death Section
public:
	UFUNCTION()
	virtual void Death(AActor* CausedActor) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<USoundBase> DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<USoundBase> HurtSound;
};
