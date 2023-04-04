// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicFloor.h"
#include "StairDownFloor.generated.h"

UCLASS()
class COLLAPSING_API AStairDownFloor : public ABasicFloor
{
	GENERATED_BODY()

public:
	AStairDownFloor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UStaticMeshComponent>> StairArray;

};
