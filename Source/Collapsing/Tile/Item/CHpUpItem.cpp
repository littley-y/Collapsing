// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/Item/CHpUpItem.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/CCharacterInteractionInterface.h"

ACHpUpItem::ACHpUpItem()
{
	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMeshComponent);

	ItemMeshComponent->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));
	ItemMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ACHpUpItem::OnPlayerItemOverlap);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> AppleMeshRef(
		TEXT("/Script/Engine.StaticMesh'/Game/_GameAssets/Meshes/Foods/Food_Fruit_Apple_01.Food_Fruit_Apple_01'"));
	if (IsValid(AppleMeshRef.Object))
	{
		FruitsMeshes.Add(AppleMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> OrangeMeshRef(
		TEXT("/Script/Engine.StaticMesh'/Game/_GameAssets/Meshes/Foods/Food_Fruit_Orange_01.Food_Fruit_Orange_01'"));

	if (IsValid(OrangeMeshRef.Object))
	{
		FruitsMeshes.Add(OrangeMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PineappleMeshRef(
		TEXT("/Script/Engine.StaticMesh'/Game/_GameAssets/Meshes/Foods/Food_Fruit_Pineapple_01.Food_Fruit_Pineapple_01'"));

	if (IsValid(PineappleMeshRef.Object))
	{
		FruitsMeshes.Add(PineappleMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WatermelonMeshRef(
		TEXT("/Script/Engine.StaticMesh'/Game/_GameAssets/Meshes/Foods/Food_Fruit_Watermelon_01.Food_Fruit_Watermelon_01'"));
	if (IsValid(WatermelonMeshRef.Object))
	{
		FruitsMeshes.Add(WatermelonMeshRef.Object);
	}
}


void ACHpUpItem::OnPlayerItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ICCharacterInteractionInterface* RunCharacter = Cast<ICCharacterInteractionInterface>(OtherActor);
	if (RunCharacter != nullptr && OtherComp != nullptr)
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

	ItemMeshComponent->SetStaticMesh(FruitsMeshes[FMath::RandRange(0, 3)]);
	ItemMeshComponent->SetRelativeScale3D({1.5f, 1.5f, 1.5f});
	ItemMeshComponent->SetSimulatePhysics(true);
	ItemMeshComponent->SetEnableGravity(false);
	ItemMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemMeshComponent->SetCollisionProfileName(TEXT("ItemCollision"));

	const float X = FMath::RandBool() ? FMath::FRandRange(1000.f, 2000.f) : FMath::FRandRange(-2000.f, -1000.f);
	const float Y = FMath::RandBool() ? FMath::FRandRange(1000.f, 2000.f) : FMath::FRandRange(-2000.f, -1000.f);
	const float Z = FMath::RandBool() ? FMath::FRandRange(1000.f, 2000.f) : FMath::FRandRange(-2000.f, -1000.f);
	const FVector TargetLocation(X, Y, Z);

	ItemMeshComponent->AddImpulse(TargetLocation);
}
