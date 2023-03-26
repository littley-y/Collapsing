// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicFloor.h"
#include "CornerFloor.generated.h"

/**
 * 
 */
UCLASS()
class COLLAPSING_API ACornerFloor : public ABasicFloor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TriggerBox", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TurnZone;

public:

	ACornerFloor();

	UFUNCTION()
	void OnPlayerTurnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	virtual void SetMeshLocation() const override final;
};
