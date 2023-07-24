// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile/CBasicFloor.h"
#include "CObstacleFloor.generated.h"

UENUM()
enum EObstacleType
{
	Bed_Drawer = 0,
	Bed_Table,
	Footrest,
	Trash_Can,
	Closet,
	Sofa,
	LaundryShelf
};

/**
 * 
 */
UCLASS()
class COLLAPSING_API ACObstacleFloor : public ACBasicFloor
{
	GENERATED_BODY()

public:
	ACObstacleFloor();

	UFUNCTION()
	void OnObstacleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                   FVector NormalImpulse, const FHitResult& Hit);

	virtual void ActivateFloor() override;
	virtual void DeactivateFloor() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Obstacle, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Obstacle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Obstacle, meta = (AllowPrivateAccess = "true"))
	TArray<FVector> WallObstacleLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Obstacle, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UStaticMesh>> ObstacleMeshes;
};
