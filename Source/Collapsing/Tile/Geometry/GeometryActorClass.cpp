// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/Geometry/GeometryActorClass.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

AGeometryActorClass::AGeometryActorClass()
{
	GetGeometryCollectionComponent()->SetCollisionProfileName("OverlapOnlyPawn");
	GetGeometryCollectionComponent()->SetEnableGravity(false);
	AActor::SetLifeSpan(2.f);
}
