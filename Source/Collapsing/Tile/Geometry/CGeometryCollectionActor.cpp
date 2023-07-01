// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/Geometry/CGeometryCollectionActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

ACGeometryCollectionActor::ACGeometryCollectionActor()
{
	GetGeometryCollectionComponent()->SetCollisionProfileName("OverlapOnlyPawn");
	GetGeometryCollectionComponent()->SetEnableGravity(false);
	AActor::SetLifeSpan(2.f);
}
