// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "BasicObstacle.generated.h"

/**
 * 
 */
UCLASS()
class COLLAPSING_API UBasicObstacle : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UBasicObstacle();

	UFUNCTION()
	void OnObstacleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	               FVector NormalImpulse, const FHitResult& Hit);

protected:

};
