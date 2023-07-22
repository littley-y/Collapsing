// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "CGeometryCollectionActor.generated.h"

/**
 * 
 */
UCLASS()
class COLLAPSING_API ACGeometryCollectionActor : public AGeometryCollectionActor
{
	GENERATED_BODY()

public:
	ACGeometryCollectionActor();

	UFUNCTION()
	void OnPlayerDeathOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                     int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPointLightComponent> CeilingLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "Ceiling", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> CeilingLightMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TriggerBox", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> CharacterDeathZone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DeathCam", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> DeathCam;


};
