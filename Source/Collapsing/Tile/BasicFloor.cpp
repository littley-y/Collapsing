// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicFloor.h"
#include "Character/RunCharacter.h"
#include "Utils/AssetFinder.hpp"
#include "Item/CHpUpItem.h"

ABasicFloor::ABasicFloor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	check(SceneComponent)

	RootComponent = SceneComponent;
	CreateFloor();
	CreateWallArray();
	CreateItem();
}

void ABasicFloor::CreateFloor()
{
	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(SceneComponent);
	FloorMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	static UStaticMesh* FloorMeshAsset = MyFunction::AssetObjectFinder<UStaticMesh>(
		TEXT("/Game/_GameAssets/Meshes/Floor_400x400"));
	if (IsValid(FloorMeshAsset))
	{
		FloorMesh->SetStaticMesh(FloorMeshAsset);
	}
}

void ABasicFloor::CreateWallArray()
{
	WallArray.SetNum(2);
	for (int32 Ix = 0; Ix != WallArray.Num(); ++Ix)
	{
		SetWall(WallArray[Ix], Ix);
	}
}

void ABasicFloor::SetWall(TObjectPtr<UStaticMeshComponent>& Wall, const int8 Ix)
{
	FString MeshName(TEXT("WallMesh"));
	MeshName += FString::FromInt(Ix);

	Wall = CreateDefaultSubobject<UStaticMeshComponent>(*MeshName);
	if (!IsValid(Wall))
	{
		return;
	}
	Wall->SetupAttachment(FloorMesh);
	Wall->SetCollisionProfileName("BlockAll");
	Wall->SetRelativeLocation(FVector(0.f, 0.f, -20.f));
	Wall->OnComponentHit.AddDynamic(this, &ABasicFloor::OnWallHit);

	static UStaticMesh* WallMeshAsset = MyFunction::AssetObjectFinder<UStaticMesh>(
		TEXT("/Game/_GameAssets/Meshes/Wall_400x200"));
	if (IsValid(WallMeshAsset))
	{
		Wall->SetStaticMesh(WallMeshAsset);
	}

	if (Ix == 1)
	{
		WallArray[Ix]->SetRelativeLocation(FVector(0.f, 400.f, -20.f));
	}
}

void ABasicFloor::CreateItem()
{
	HpUpItem = CreateDefaultSubobject<UCHpUpItem>(TEXT("HpUpItem"));
	if (HpUpItem)
	{
		HpUpItem->SetupAttachment(WallArray[0]);
		HpUpItem->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	}
}

void ABasicFloor::OnWallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
							FVector NormalImpulse, const FHitResult& Hit)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	if (IsValid(RunCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Contacted With Wall"))
		RunCharacter->Death();
	}
}
