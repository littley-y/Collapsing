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

public:
	ACornerFloor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "TriggerBox")
	class UBoxComponent* TurnZone;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	virtual void SetMeshLocation() const override final;
};
