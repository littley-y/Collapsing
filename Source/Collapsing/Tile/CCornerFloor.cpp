// Fill out your copyright notice in the Description page of Project Settings.

#include "CCornerFloor.h"
#include "Collapsing/Character/CCharacter.h"
#include "Components/BoxComponent.h"

ACCornerFloor::ACCornerFloor()
{
	SetTurnZone();

	Walls[0]->OnComponentHit.AddDynamic(this, &ACCornerFloor::OnWallHit);
	Walls[1]->OnComponentHit.AddDynamic(this, &ACCornerFloor::OnWallHit);
}

void ACCornerFloor::SetTurnZone()
{
	TurnZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TurnZone"));
	if (IsValid(TurnZone))
	{
		TurnZone->SetupAttachment(FloorMesh);
		TurnZone->SetBoxExtent(FVector(150.f, 150.f, 10.f));
		TurnZone->SetRelativeLocation(FVector(200.f, 200.f, 0.f));

		TurnZone->SetGenerateOverlapEvents(true);
		TurnZone->OnComponentBeginOverlap.AddDynamic(this, &ACCornerFloor::OnPlayerTurnOverlap);
		TurnZone->OnComponentEndOverlap.AddDynamic(this, &ACCornerFloor::OnPlayerEndOverlap);
	}
}

void ACCornerFloor::OnPlayerTurnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACCharacter* RunCharacter = Cast<ACCharacter>(OtherActor);
	if (IsValid(RunCharacter) && OtherComp)
	{
		RunCharacter->bCanCharacterTurn = true;
	}
}

void ACCornerFloor::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACCharacter* RunCharacter = Cast<ACCharacter>(OtherActor);
	if (IsValid(RunCharacter) && OtherComp)
	{
		RunCharacter->bCanCharacterTurn = false;
	}
}

void ACCornerFloor::OnWallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	ACCharacter* RunCharacter = Cast<ACCharacter>(OtherActor);
	if (IsValid(RunCharacter))
	{
		const float Dot = FVector::DotProduct(Hit.Normal, RunCharacter->GetActorForwardVector());
		if (Dot > 0.99f && Dot < 1.01f)
		{
			RunCharacter->HitByWall();
		}
	}
}

