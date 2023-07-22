// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile/CBasicFloor.h"
#include "CBrokenFloor.generated.h"

/**
 * 
 */
UCLASS()
class COLLAPSING_API ACBrokenFloor : public ACBasicFloor
{
	GENERATED_BODY()

public:
	ACBrokenFloor();

	UFUNCTION()
	void OnPlayerDeathOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                     int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TriggerBox", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> CharacterDeathZone;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> DeathCam;
	
};
