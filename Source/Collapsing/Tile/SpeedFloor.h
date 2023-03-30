// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicFloor.h"
#include "SpeedFloor.generated.h"

/**
 * 
 */
UCLASS()
class COLLAPSING_API ASpeedFloor : public ABasicFloor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TriggerBox", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> SpeedZone;

public:
	ASpeedFloor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* ArrowMesh;

	UFUNCTION()
	void OnPlayerSpeedOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                          int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
