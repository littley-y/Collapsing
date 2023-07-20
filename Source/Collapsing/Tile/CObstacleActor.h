// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CObstacleActor.generated.h"

UCLASS()
class COLLAPSING_API ACObstacleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACObstacleActor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Obstacle, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Obstacle;


};
