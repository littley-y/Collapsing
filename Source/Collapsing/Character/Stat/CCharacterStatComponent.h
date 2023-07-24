// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpZeroDelegate, AActor* /* Death Type */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /* Current Hp */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDistanceChangedDelegate, int32 /* Changed Distance */)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COLLAPSING_API UCCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// Hp Section
public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;

	UCCharacterStatComponent();

	FORCEINLINE float GetMaxHp() const { return MaxHp; }
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }

	float ApplyDamage(float InDamage);

	void SetHp(float NewHp);

protected:
	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

private:
	FTimerHandle HpHandler;

	// Distance Section
public:
	void AddTraveledDistance();
	FOnDistanceChangedDelegate OnDistanceChanged;

private:
	FVector LastLocation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Score", meta = (AllowPrivateAccess = "true"))
	int32 TraveledDistance;
};
