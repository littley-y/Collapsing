// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/Item/CHpUpItem.h"
#include "Utils/AssetFinder.hpp"
#include "Character/RunCharacter.h"

UCHpUpItem::UCHpUpItem()
{
	UStaticMesh* ItemMesh = MyFunction::AssetObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	SetStaticMesh(ItemMesh);
	SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	SetCollisionProfileName("OvelapOnlyPawn");

	OnComponentHit.AddDynamic(this, &UCHpUpItem::OnItemHit);
}

void UCHpUpItem::OnItemHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	if (IsValid(RunCharacter) && OtherComp)
	{
		RunCharacter->GetHpUpItem();
		DestroyComponent();
	}
}
