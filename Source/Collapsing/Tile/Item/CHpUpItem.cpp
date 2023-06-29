// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile/Item/CHpUpItem.h"
#include "Character/RunCharacter.h"

UCHpUpItem::UCHpUpItem()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ItemMeshRef(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (IsValid(ItemMeshRef.Object))
	{
		UStaticMeshComponent::SetStaticMesh(ItemMeshRef.Object);
	}
	SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));

	OnComponentBeginOverlap.AddDynamic(this, &UCHpUpItem::OnPlayerItemOverlap);
}

void UCHpUpItem::OnPlayerItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	const ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	if (IsValid(RunCharacter) && OtherComp)
	{
		RunCharacter->EarnHpUpItem();
		DestroyComponent();
	}
}
