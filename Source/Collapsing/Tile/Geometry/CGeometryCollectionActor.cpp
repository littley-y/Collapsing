// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile/Geometry/CGeometryCollectionActor.h"
#include "Components/BoxComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Camera/CameraComponent.h"
#include "Interface/CCharacterInteractionInterface.h"

ACGeometryCollectionActor::ACGeometryCollectionActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);

	GeometryCollectionComponent->SetCollisionProfileName("OverlapOnlyPawn");
	GeometryCollectionComponent->SetEnableGravity(false);

	CharacterDeathZone = CreateDefaultSubobject<UBoxComponent>(TEXT("CharacterDeathZone"));

	CharacterDeathZone->SetupAttachment(GetGeometryCollectionComponent());
	CharacterDeathZone->SetBoxExtent(FVector(400.f, 400.f, 10.f));
	CharacterDeathZone->SetRelativeLocation(FVector(400.f, 400.f, -55.f));

	CharacterDeathZone->SetGenerateOverlapEvents(true); 
	CharacterDeathZone->OnComponentBeginOverlap.AddDynamic(this, &ACGeometryCollectionActor::OnPlayerDeathOverlap);

	DeathCam = CreateDefaultSubobject<UCameraComponent>(TEXT("DeathCam"));
	DeathCam->SetupAttachment(CharacterDeathZone);
	DeathCam->SetFieldOfView(120.f);
	DeathCam->SetRelativeLocationAndRotation({100.f, 100.f, -100.f}, {-90.f, 180.f, 0.f});
}

void ACGeometryCollectionActor::OnPlayerDeathOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ICCharacterInteractionInterface* RunCharacter = Cast<ICCharacterInteractionInterface>(OtherActor);
	if (RunCharacter != nullptr && OtherComp)
	{
		DeathCam->Activate();
		SetActorEnableCollision(false);
		RunCharacter->Death(this);
	}
}

void ACGeometryCollectionActor::BeginPlay()
{
	Super::BeginPlay();

	DeathCam->Deactivate();
}
