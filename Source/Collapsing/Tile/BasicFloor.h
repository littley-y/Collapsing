// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicFloor.generated.h"

class UTileGenerator;

UCLASS()
class COLLAPSING_API ABasicFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Scene")
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StaticMeshComponent")
	TObjectPtr<UStaticMeshComponent> FloorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Array")
	TArray<TObjectPtr<UStaticMeshComponent>> WallArray;

	ABasicFloor();

	UFUNCTION()
	void OnWallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			       FVector NormalImpulse, const FHitResult& Hit);

protected:
	virtual void BeginPlay() override;

	void SetWallArray();

	virtual void SetWall(TObjectPtr<UStaticMeshComponent>& Wall, const int8 Ix);

	virtual void CreateFloor();
};

