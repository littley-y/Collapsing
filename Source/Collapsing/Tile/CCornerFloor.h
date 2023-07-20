// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CBasicFloor.h"
#include "CCornerFloor.generated.h"

UCLASS()
class COLLAPSING_API ACCornerFloor : public ACBasicFloor
{
	GENERATED_BODY()

public:
	ACCornerFloor();

	UFUNCTION()
	void OnPlayerTurnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                         int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TriggerBox", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> TurnZone;
};
