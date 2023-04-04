// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicFloor.h"
#include "SpeedFloor.generated.h"

UCLASS()
class COLLAPSING_API ASpeedFloor : public ABasicFloor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* ArrowMesh;

	ASpeedFloor();

	UFUNCTION()
	void OnPlayerSpeedOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                          int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TriggerBox", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> SpeedZone;

	void SetSpeedZone();
};
