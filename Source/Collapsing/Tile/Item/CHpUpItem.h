// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "CHpUpItem.generated.h"

/**
 * 
 */
UCLASS()
class COLLAPSING_API UCHpUpItem : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UCHpUpItem();

protected:
	UFUNCTION()
	void OnPlayerItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                         int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
