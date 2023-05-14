// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/Geometry/GeometryLeftCornerFloor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

AGeometryLeftCornerFloor::AGeometryLeftCornerFloor()
{
	GetGeometryCollectionComponent()->SetCollisionProfileName("OverlapOnlyPawn");
	AActor::SetLifeSpan(2.f);
}
