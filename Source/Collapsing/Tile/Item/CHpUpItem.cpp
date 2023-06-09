// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/Item/CHpUpItem.h"

#include "Character/CCharacter.h"
#include "Kismet/GameplayStatics.h"

ACHpUpItem::ACHpUpItem()
{
	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMeshComponent);

	ItemMeshComponent->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));
	ItemMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ACHpUpItem::OnPlayerItemOverlap);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ItemMeshRef(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (IsValid(ItemMeshRef.Object))
	{
		ItemMeshComponent->SetStaticMesh(ItemMeshRef.Object);
	}
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
	ItemMeshComponent->SetEnableGravity(false);
	ItemMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemMeshComponent->SetCollisionProfileName(TEXT("ItemCollision"));

	const float X = FMath::RandBool() ? FMath::FRandRange(2000.f, 4000.f) : FMath::FRandRange(-4000.f, -2000.f);
	const float Y = FMath::RandBool() ? FMath::FRandRange(2000.f, 4000.f) : FMath::FRandRange(-4000.f, -2000.f);
	const float Z = FMath::RandBool() ? FMath::FRandRange(2000.f, 4000.f) : FMath::FRandRange(-4000.f, -2000.f);
	const FVector TargetLocation(X, Y, Z);
	ItemMeshComponent->AddImpulse(TargetLocation);
}
