// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/Item/CHpUpItem.h"

#include "Character/CCharacter.h"
#include "Kismet/GameplayStatics.h"

ACHpUpItem::ACHpUpItem()
{
	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ItemMeshRef(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (IsValid(ItemMeshRef.Object))
	{
		ItemMeshComponent->SetStaticMesh(ItemMeshRef.Object);
	}
	ItemMeshComponent->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));
	ItemMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ACHpUpItem::OnPlayerItemOverlap);
	ItemMeshComponent->SetEnableGravity(false);
}


void ACHpUpItem::OnPlayerItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const ACCharacter* RunCharacter = Cast<ACCharacter>(OtherActor);
	if (IsValid(RunCharacter) && OtherComp)
	{
		RunCharacter->EarnHpUpItem();
		if (IsValid(PickSound))
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickSound, GetActorLocation());
		}
		SetActorHiddenInGame(true);
		SetLifeSpan(1.f);
	}
}

void ACHpUpItem::BeginPlay()
{
	Super::BeginPlay();

	ItemMeshComponent->SetSimulatePhysics(true);
	ItemMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemMeshComponent->SetCollisionProfileName(TEXT("ItemCollision"));

	const FVector TargetLocation(FMath::FRandRange(2000.f, 4000.f), FMath::FRandRange(2000.f, 4000.f),
	                             FMath::FRandRange(2000.f, 4000.f));
	ItemMeshComponent->AddImpulse(TargetLocation);
}
