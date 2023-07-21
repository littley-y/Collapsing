// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CInitTile.generated.h"

UCLASS()
class COLLAPSING_API ACInitTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACInitTile();

	void OnPlayerDoorEntered(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                         int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnPlayerDoorExited(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                         int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Floor")
	TObjectPtr<UStaticMeshComponent> Floor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Wall")
	TObjectPtr<UStaticMeshComponent> Wall1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TriggerBox", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> StageDoorTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Wall")
	TObjectPtr<UStaticMeshComponent> Wall2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Wall")
	TObjectPtr<UStaticMeshComponent> Wall3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TriggerBox", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> QuitDoorTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Wall")
	TObjectPtr<UStaticMeshComponent> Wall4;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Ceiling")
	TObjectPtr<UStaticMeshComponent> Ceiling;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Ceiling")
	TObjectPtr<class UPointLightComponent> CeilingLight;



};
