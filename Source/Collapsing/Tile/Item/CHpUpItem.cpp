// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile/Item/CHpUpItem.h"
#include "Character/RunCharacter.h"
#include "Utils/AssetFinder.hpp"

UCHpUpItem::UCHpUpItem()
{
	UStaticMesh* ItemMesh = MyFunction::AssetObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	UStaticMeshComponent::SetStaticMesh(ItemMesh);
	SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	OnComponentBeginOverlap.AddDynamic(this, &UCHpUpItem::OnPlayerItemOverlap);
}

void UCHpUpItem::OnPlayerItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                     const FHitResult& SweepResult)
{
	const ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	if (IsValid(RunCharacter) && OtherComp)
	{
		RunCharacter->GetHpUpItem();
		DestroyComponent();
	}
}
