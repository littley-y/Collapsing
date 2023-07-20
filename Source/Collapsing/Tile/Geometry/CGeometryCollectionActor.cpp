// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile/Geometry/CGeometryCollectionActor.h"
#include "Components/BoxComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Interface/CCharacterInteractionInterface.h"

ACGeometryCollectionActor::ACGeometryCollectionActor()
{
	GeometryCollectionComponent->SetCollisionProfileName("OverlapOnlyPawn");
	GeometryCollectionComponent->SetEnableGravity(false);

	CharacterDeathZone = CreateDefaultSubobject<UBoxComponent>(TEXT("CharacterDeathZone"));

	CharacterDeathZone->SetupAttachment(GetGeometryCollectionComponent());
	CharacterDeathZone->SetBoxExtent(FVector(400.f, 400.f, 10.f));
	CharacterDeathZone->SetRelativeLocation(FVector(400.f, 400.f, -400.f));

	CharacterDeathZone->SetGenerateOverlapEvents(true); 
	CharacterDeathZone->OnComponentBeginOverlap.AddDynamic(this, &ACGeometryCollectionActor::OnPlayerDeathOverlap);
}

void ACGeometryCollectionActor::OnPlayerDeathOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ICCharacterInteractionInterface* RunCharacter = Cast<ICCharacterInteractionInterface>(OtherActor);
	if (RunCharacter != nullptr && OtherComp)
	{
		RunCharacter->Death();
	}
}
