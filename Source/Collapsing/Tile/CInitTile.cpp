// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile/CInitTile.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Interface/CCharacterInteractionInterface.h"
#include "Game/CollapsingGameMode.h"

ACInitTile::ACInitTile()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));

	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	Floor->SetupAttachment(SceneComponent);

	SetWalls();
	SetCeiling();
}

void ACInitTile::SetWalls()
{
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
	StageDoorTrigger->SetBoxExtent({ 80.f, 80.f, 10.f });

	StageDoorTrigger->SetGenerateOverlapEvents(true);
	StageDoorTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACInitTile::OnPlayerDoorEntered);
	StageDoorTrigger->OnComponentEndOverlap.AddDynamic(this, &ACInitTile::OnPlayerDoorExited);

	StageDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StageDoor"));
	StageDoor->SetupAttachment(Wall1);

	QuitDoorTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("QuitDoorTrigger"));
	QuitDoorTrigger->SetupAttachment(Wall3);
	QuitDoorTrigger->SetBoxExtent({ 80.f, 80.f, 10.f });

	QuitDoorTrigger->SetGenerateOverlapEvents(true);
	QuitDoorTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACInitTile::OnPlayerDoorEntered);
	QuitDoorTrigger->OnComponentEndOverlap.AddDynamic(this, &ACInitTile::OnPlayerDoorExited);

	QuitDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("QuitDoor"));
	QuitDoor->SetupAttachment(Wall3);
}

void ACInitTile::OnPlayerDoorEntered(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	ICCharacterInteractionInterface* RunCharacter = Cast<ICCharacterInteractionInterface>(OtherActor);
	if (RunCharacter != nullptr && OtherComp)
	{
		RunCharacter->SetCanOpenDoor(true);
		UE_LOG(LogTemp, Warning, TEXT("RunCharacter Valid"))
	}
}

void ACInitTile::OnPlayerDoorExited(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ICCharacterInteractionInterface* RunCharacter = Cast<ICCharacterInteractionInterface>(OtherActor);
	if (RunCharacter != nullptr && OtherComp)
	{
		RunCharacter->SetCanOpenDoor(false);
		UE_LOG(LogTemp, Warning, TEXT("RunCharacter Valid"))
	}
}

void ACInitTile::SetCeiling()
{
	Ceiling = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ceiling"));
	Ceiling->SetupAttachment(SceneComponent);

	CeilingLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("CeilingLight"));
	CeilingLight->SetupAttachment(Ceiling);

	CeilingLight->SetRelativeLocation({ 200.f, 200.f, -50.f });
	CeilingLight->SetIntensity(20000.f);
	CeilingLight->AttenuationRadius = 1500.f;
	CeilingLight->SetUseTemperature(true);
	CeilingLight->Temperature = 5500.f;
}
