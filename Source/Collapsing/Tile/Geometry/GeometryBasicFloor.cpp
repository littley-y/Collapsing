// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/Geometry/GeometryBasicFloor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

AGeometryBasicFloor::AGeometryBasicFloor()
{
	GetGeometryCollectionComponent()->SetCollisionProfileName("OverlapOnlyPawn");
	AActor::SetLifeSpan(2.f);
}
