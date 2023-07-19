// Fill out your copyright notice in the Description page of Project Settings.

#include "CBasicObstacle.h"
#include "Interface/CCharacterInteractionInterface.h"

UCBasicObstacle::UCBasicObstacle()
{
	UStaticMeshComponent::SetCollisionProfileName("BlockNotCamera");
	OnComponentHit.AddDynamic(this, &UCBasicObstacle::OnObstacleHit);
}

void UCBasicObstacle::OnObstacleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ICCharacterInteractionInterface* RunCharacter = Cast<ICCharacterInteractionInterface>(OtherActor);
	if (RunCharacter != nullptr)
	{
		RunCharacter->HitBySomething();
	}
}
