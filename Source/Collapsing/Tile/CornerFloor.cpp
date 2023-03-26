// Fill out your copyright notice in the Description page of Project Settings.


#include "CornerFloor.h"
#include "Collapsing/Character/RunCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

ACornerFloor::ACornerFloor()
{
	ACornerFloor::SetMeshLocation();

	TurnZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TurnZone"));
	TurnZone->SetupAttachment(SceneComponent);
	TurnZone->SetBoxExtent(FVector(150.f, 150.f, 10.f));
	TurnZone->SetRelativeLocation(FVector(200.f, 200.f, 0.f));

	TurnZone->SetGenerateOverlapEvents(true);
	TurnZone->OnComponentBeginOverlap.AddDynamic(this, &ACornerFloor::OnPlayerTurnOverlap);

}

void ACornerFloor::OnPlayerTurnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	if (RunCharacter != nullptr && OtherComp)
	{
		RunCharacter->bCanTurn = true;
	}
}

void ACornerFloor::SetMeshLocation() const
{
	FloorMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	LeftWallMesh->SetRelativeLocation(FVector(0.f, 0.f, -20.f));
	RightWallMesh->SetRelativeLocation(FVector(400.f, 0.f, -20.f));
	RightWallMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
}
