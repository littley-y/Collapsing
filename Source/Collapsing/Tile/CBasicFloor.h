// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBasicFloor.generated.h"

UCLASS()
class COLLAPSING_API ACBasicFloor : public AActor
{
	GENERATED_BODY()
	
public:
	ACBasicFloor();

	virtual void ActivateFloor();
	virtual void DeactivateFloor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Floor")
	TObjectPtr<UStaticMeshComponent> FloorMesh;

// Wall Section
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Wall")
	TObjectPtr<UStaticMeshComponent> LeftWall;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Wall")
	TObjectPtr<UStaticMeshComponent> RightWall;

	UFUNCTION()
	void OnWallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	               FVector NormalImpulse, const FHitResult& Hit);

// Ceiling Section
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Ceiling")
	TObjectPtr<UStaticMeshComponent> CeilingMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Ceiling")
	TObjectPtr<class UPointLightComponent> CeilingLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Ceiling")
	TObjectPtr<UStaticMeshComponent> CeilingLightMesh;

// Item Section
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<class ACHpUpItem> BP_HpUpItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<AActor> SpawnedHpUpItem;

protected:
	void CreateFloor();
	void CreateWalls();
	void CreateCeiling();
};

