// Fill out your copyright notice in the Description page of Project Settings.

#include "CSpeedFloor.h"
#include "Collapsing/Character/CCharacter.h"
#include "Components/BoxComponent.h"

ACSpeedFloor::ACSpeedFloor()
{
	ArrowMesh = CreateDefaultSubobject<UStaticMesh>(TEXT("ArrowMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/_GameAssets/Meshes/SM_Arrows.SM_Arrows'"));
	if (ArrowMeshRef.Succeeded())
	{
		ArrowMesh = ArrowMeshRef.Object;
	}

	UpArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UpArrowMesh"));
	UpArrowMesh->SetupAttachment(SceneComponent);
	UpArrowMesh->SetCollisionProfileName("OverlapPawnOnly");
	UpArrowMesh->SetRelativeLocation(FVector(200.f, 50.f, 50.f));

	DownArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DownArrowMesh"));
	DownArrowMesh->SetupAttachment(SceneComponent);
	DownArrowMesh->SetCollisionProfileName("OverlapPawnOnly");
	DownArrowMesh->SetRelativeLocation(FVector(200.f, 50.f, 50.f));

	if (IsValid(ArrowMesh))
	{
		if (UpArrowMesh.IsPending())
		{
			//UpArrowMesh.LoadSynchronous();
			//DownArrowMesh.LoadSynchronous();
		}
		UpArrowMesh->SetStaticMesh(ArrowMesh);
		DownArrowMesh->SetStaticMesh(ArrowMesh);
	}

	SetSpeedZone();
}

void ACSpeedFloor::OnPlayerSpeedOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACCharacter* RunCharacter = Cast<ACCharacter>(OtherActor);
	if (IsValid(RunCharacter) && OtherComp != nullptr)
	{
		RunCharacter->bCanChangeSpeed = true;
	}
}

void ACSpeedFloor::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACCharacter* RunCharacter = Cast<ACCharacter>(OtherActor);
	if (IsValid(RunCharacter) && OtherComp != nullptr)
	{
		RunCharacter->bCanChangeSpeed = false;
	}
}

void ACSpeedFloor::SetSpeedZone()
{
	SpeedZone = CreateDefaultSubobject<UBoxComponent>(TEXT("SpeedZone"));
	if (IsValid(SpeedZone))
	{
		SpeedZone->SetupAttachment(SceneComponent);
		SpeedZone->SetBoxExtent(FVector(150.f, 150.f, 10.f));
		SpeedZone->SetRelativeLocation(FVector(200.f, 200.f, 0.f));

		SpeedZone->SetGenerateOverlapEvents(true);
		SpeedZone->OnComponentBeginOverlap.AddDynamic(this, &ACSpeedFloor::OnPlayerSpeedOverlap);
		SpeedZone->OnComponentEndOverlap.AddDynamic(this, &ACSpeedFloor::OnPlayerEndOverlap);
	}
}
