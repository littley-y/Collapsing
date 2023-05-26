// Fill out your copyright notice in the Description page of Project Settings.

#include "CornerFloor.h"
#include "Collapsing/Character/RunCharacter.h"
#include "Components/BoxComponent.h"
#include "Item/CHpUpItem.h"

ACornerFloor::ACornerFloor()
{
	SetTurnZone();
	HpUpItem->DestroyComponent();
}

void ACornerFloor::SetTurnZone()
{
	TurnZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TurnZone"));
	if (IsValid(TurnZone))
	{
		TurnZone->SetupAttachment(FloorMesh);
		TurnZone->SetBoxExtent(FVector(150.f, 150.f, 10.f));
		TurnZone->SetRelativeLocation(FVector(200.f, 200.f, 0.f));

		TurnZone->SetGenerateOverlapEvents(true);
		TurnZone->OnComponentBeginOverlap.AddDynamic(this, &ACornerFloor::OnPlayerTurnOverlap);
		TurnZone->OnComponentEndOverlap.AddDynamic(this, &ACornerFloor::OnPlayerEndOverlap);
	}
}

void ACornerFloor::OnPlayerTurnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	if (IsValid(RunCharacter) && OtherComp)
	{
		RunCharacter->bCanCharacterTurn = true;
	}
}

void ACornerFloor::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	if (IsValid(RunCharacter) && OtherComp)
	{
		RunCharacter->bCanCharacterTurn = false;
	}
}

