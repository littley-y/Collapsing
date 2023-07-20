// Fill out your copyright notice in the Description page of Project Settings.

#include "CCornerFloor.h"
#include "Components/BoxComponent.h"
#include "Interface/CCharacterInteractionInterface.h"

ACCornerFloor::ACCornerFloor()
{
	TurnZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TurnZone"));
	if (IsValid(TurnZone))
	{
		TurnZone->SetupAttachment(FloorMesh);
		TurnZone->SetBoxExtent(FVector(200.f, 200.f, 10.f));
		TurnZone->SetRelativeLocation(FVector(200.f, 200.f, 0.f));

		TurnZone->SetGenerateOverlapEvents(true);
		TurnZone->OnComponentBeginOverlap.AddDynamic(this, &ACCornerFloor::OnPlayerTurnOverlap);
	}
}

void ACCornerFloor::OnPlayerTurnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ICCharacterInteractionInterface* RunCharacter = Cast<ICCharacterInteractionInterface>(OtherActor);
	if (RunCharacter != nullptr && OtherComp)
	{
		RunCharacter->SetTurnStatus(true);
	}
}
