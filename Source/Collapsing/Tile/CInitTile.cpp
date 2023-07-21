// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile/CInitTile.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Interface/CCharacterInteractionInterface.h"

ACInitTile::ACInitTile()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));

	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	Floor->SetupAttachment(SceneComponent);

	Wall1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall1"));
	Wall1->SetupAttachment(SceneComponent);

	Wall2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall2"));
	Wall2->SetupAttachment(SceneComponent);
	
	Wall3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall3"));
	Wall3->SetupAttachment(SceneComponent);
	
	Wall4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall4"));
	Wall4->SetupAttachment(SceneComponent);

	StageDoorTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageDoorTrigger"));
	StageDoorTrigger->SetupAttachment(Wall1);

	QuitDoorTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("QuitDoorTrigger"));
	QuitDoorTrigger->SetupAttachment(Wall3);

	Ceiling = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ceiling"));
	Ceiling->SetupAttachment(SceneComponent);

	CeilingLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("CeilingLight"));
	CeilingLight->SetupAttachment(Ceiling);

	CeilingLight->SetRelativeLocation({200.f, 200.f, -300.f});
	CeilingLight->SetIntensity(30000.f);
	CeilingLight->AttenuationRadius = 1500.f;
	CeilingLight->SetUseTemperature(true);
	CeilingLight->Temperature = 5500.f;


}

void ACInitTile::OnPlayerDoorEntered(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ICCharacterInteractionInterface* RunCharacter = Cast<ICCharacterInteractionInterface>(OtherActor);
	if (RunCharacter)
	{
		RunCharacter->OpenDoor();
	}
}

void ACInitTile::OnPlayerDoorExited(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ICCharacterInteractionInterface* RunCharacter = Cast<ICCharacterInteractionInterface>(OtherActor);
	if (RunCharacter)
	{
		RunCharacter->OpenDoor();
	}
}


