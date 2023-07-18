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

	void ActivateFloor();
	void DeactivateFloor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Floor")
	TObjectPtr<UStaticMeshComponent> FloorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Obstacle")
	TArray<TObjectPtr<UStaticMeshComponent>> Obstacles;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Wall")
	TArray<TObjectPtr<UStaticMeshComponent>> Walls;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Ceiling")
	TObjectPtr<UStaticMeshComponent> CeilingMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Ceiling")
	TObjectPtr<class UPointLightComponent> CeilingLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Ceiling")
	TObjectPtr<UStaticMeshComponent> CeilingLightMesh;

// Item Section
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Item)
	TSubclassOf<class ACHpUpItem> BP_HpUpItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Item)
	TObjectPtr<AActor> SpawnedHpUpItem;

protected:
	virtual void BeginPlay() override;

	void CreateCeiling();

	void CreateFloor();

	void CreateWalls();
	void SetWall(TObjectPtr<UStaticMeshComponent>& Wall, const int8 Ix);
};

