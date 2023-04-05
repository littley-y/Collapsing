// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedFloor.h"
#include "Collapsing/Character/RunCharacter.h"
#include "Components/BoxComponent.h"

ASpeedFloor::ASpeedFloor()
{
	UpArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UpArrowMesh"));
	UpArrowMesh->SetupAttachment(SceneComponent);
	UpArrowMesh->SetCollisionProfileName("OverlapPawnOnly");

	DownArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DownArrowMesh"));
	DownArrowMesh->SetupAttachment(SceneComponent);
	DownArrowMesh->SetCollisionProfileName("OverlapPawnOnly");

	SetSpeedZone();
}

void ASpeedFloor::OnPlayerSpeedOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	if (RunCharacter != nullptr && OtherComp != nullptr)
	{
		RunCharacter->bCanChangeSpeed = true;
	}
}

void ASpeedFloor::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	if (RunCharacter != nullptr && OtherComp != nullptr)
	{
		RunCharacter->bCanChangeSpeed = false;
	}
}

void ASpeedFloor::SetSpeedZone()
{
	SpeedZone = CreateDefaultSubobject<UBoxComponent>(TEXT("SpeedZone"));
	SpeedZone->SetupAttachment(SceneComponent);
	SpeedZone->SetBoxExtent(FVector(150.f, 150.f, 10.f));
	SpeedZone->SetRelativeLocation(FVector(200.f, 200.f, 0.f));

	SpeedZone->SetGenerateOverlapEvents(true);
	SpeedZone->OnComponentBeginOverlap.AddDynamic(this, &ASpeedFloor::OnPlayerSpeedOverlap);
	SpeedZone->OnComponentEndOverlap.AddDynamic(this, &ASpeedFloor::OnPlayerEndOverlap);
}
