// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile/CBrokenFloor.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "Interface/CCharacterInteractionInterface.h"

ACBrokenFloor::ACBrokenFloor()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FloorMeshRef(
		TEXT("/Script/Engine.StaticMesh'/Game/_GameAssets/Meshes/Structures/SM_BrokenFloor.SM_BrokenFloor'"));
	if (IsValid(FloorMeshRef.Object))
	{
		FloorMesh->SetStaticMesh(FloorMeshRef.Object);
	}

	CharacterDeathZone = CreateDefaultSubobject<UBoxComponent>(TEXT("CharacterDeathZone"));

	CharacterDeathZone->SetupAttachment(FloorMesh);
	CharacterDeathZone->SetBoxExtent(FVector(400.f, 400.f, 10.f));
	CharacterDeathZone->SetRelativeLocation(FVector(200.f, 200.f, -40.f));

	CharacterDeathZone->SetGenerateOverlapEvents(true); 
	CharacterDeathZone->OnComponentBeginOverlap.AddDynamic(this, &ACBrokenFloor::OnPlayerDeathOverlap);

	DeathCam = CreateDefaultSubobject<UCameraComponent>(TEXT("DeathCam"));
	DeathCam->SetupAttachment(CharacterDeathZone);
	DeathCam->SetFieldOfView(120.f);
	DeathCam->SetRelativeLocationAndRotation({100.f, 100.f, -300.f}, {-90.f, 180.f, 0.f});
}

void ACBrokenFloor::OnPlayerDeathOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ICCharacterInteractionInterface* RunCharacter = Cast<ICCharacterInteractionInterface>(OtherActor);
	if (RunCharacter != nullptr && OtherComp)
	{
		DeathCam->Activate();
		RunCharacter->Death(this);
	}
}

void ACBrokenFloor::BeginPlay()
{
	Super::BeginPlay();

	DeathCam->Deactivate();
}
