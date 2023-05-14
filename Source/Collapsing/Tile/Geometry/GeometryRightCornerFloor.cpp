// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/Geometry/GeometryRightCornerFloor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

AGeometryRightCornerFloor::AGeometryRightCornerFloor()
{
	GetGeometryCollectionComponent()->SetCollisionProfileName("OverlapOnlyPawn");
	AActor::SetLifeSpan(2.f);
}
