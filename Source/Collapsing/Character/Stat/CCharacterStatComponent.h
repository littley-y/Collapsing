// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /* Current Hp */);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLLAPSING_API UCCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;

	UCCharacterStatComponent();

	FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	float ApplyDamage(float InDamage);
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	void SetHp(float NewHp);

	virtual void BeginPlay() override;

private:
	FTimerHandle HpHandler;
};
