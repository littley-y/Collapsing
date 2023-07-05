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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Floor")
	TObjectPtr<UStaticMeshComponent> FloorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ceiling")
	TObjectPtr<UStaticMeshComponent> CeilingMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Obstacle")
	TArray<TObjectPtr<UStaticMeshComponent>> Obstacles;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Wall")
	TArray<TObjectPtr<UStaticMeshComponent>> Walls;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	TSubclassOf<class ACHpUpItem> BP_HpUpItem;

	void SetWall(TObjectPtr<UStaticMeshComponent>& Wall, const int8 Ix);

	void CreateFloorAndCeiling();
	void CreateWalls();

	virtual void BeginPlay() override;

};

