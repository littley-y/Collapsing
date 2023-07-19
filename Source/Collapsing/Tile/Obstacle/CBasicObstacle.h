// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "CBasicObstacle.generated.h"

/**
 * 
 */
UCLASS()
class COLLAPSING_API UCBasicObstacle : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UCBasicObstacle();

	UFUNCTION()
	void OnObstacleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	               FVector NormalImpulse, const FHitResult& Hit);

};
