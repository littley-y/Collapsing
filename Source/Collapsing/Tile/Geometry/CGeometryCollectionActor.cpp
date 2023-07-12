// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/Geometry/CGeometryCollectionActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

ACGeometryCollectionActor::ACGeometryCollectionActor()
{
	GeometryCollectionComponent->SetCollisionProfileName("OverlapOnlyPawn");
	GeometryCollectionComponent->SetEnableGravity(false);
}
