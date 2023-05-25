// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/Item/CHpUpItem.h"
#include "Utils/AssetFinder.hpp"
#include "Character/RunCharacter.h"

UCHpUpItem::UCHpUpItem()
{
	UStaticMesh* ItemMesh = MyFunction::AssetObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	SetStaticMesh(ItemMesh);
	SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	SetCollisionProfileName("OverlapOnlyPawn");

	OnComponentBeginOverlap.AddDynamic(this, &UCHpUpItem::OnPlayerItemOverlap);
}

void UCHpUpItem::OnPlayerItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	if (IsValid(RunCharacter) && OtherComp)
	{
		RunCharacter->GetHpUpItem();
		DestroyComponent();
	}
}
