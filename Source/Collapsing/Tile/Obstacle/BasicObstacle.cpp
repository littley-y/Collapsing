// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicObstacle.h"
#include "Character/CCharacter.h"

UBasicObstacle::UBasicObstacle()
{
	OnComponentHit.AddDynamic(this, &UBasicObstacle::OnObstacleHit);
}

void UBasicObstacle::OnObstacleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ACCharacter* RunCharacter = Cast<ACCharacter>(OtherActor);
	if (IsValid(RunCharacter))
	{
		RunCharacter->HitBySomething();
	}
}
