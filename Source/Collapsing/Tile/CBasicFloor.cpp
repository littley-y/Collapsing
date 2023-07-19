// Fill out your copyright notice in the Description page of Project Settings.

#include "CBasicFloor.h"
#include "Components/PointLightComponent.h"
#include "Item/CHpUpItem.h"

ACBasicFloor::ACBasicFloor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	check(SceneComponent);

	RootComponent = SceneComponent;
	CreateFloor();
	CreateWalls();
	CreateCeiling();
}

void ACBasicFloor::CreateFloor()
{
	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(SceneComponent);
	FloorMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	FloorMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));

	static ConstructorHelpers::FClassFinder<AActor> HpUpItemRef(TEXT("/Game/Collapsing/Tile/Item/BP_CHpUpItem"));
	if (IsValid(HpUpItemRef.Class))
	{
		BP_HpUpItem = HpUpItemRef.Class;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FloorMeshRef(
		TEXT("/Script/Engine.StaticMesh'/Game/_GameAssets/Meshes/Structure/Floor_400x400.Floor_400x400'"));
	if (IsValid(FloorMeshRef.Object))
	{
		FloorMesh->SetStaticMesh(FloorMeshRef.Object);
	}
}

void ACBasicFloor::CreateWalls()
{
	LeftWall = CreateDefaultSubobject<UStaticMeshComponent>("LeftWall");
	RightWall = CreateDefaultSubobject<UStaticMeshComponent>("RightWall");
	if (!IsValid(LeftWall) || !IsValid(RightWall))
	{
		return;
	}

	LeftWall->SetupAttachment(FloorMesh);
	LeftWall->SetCollisionProfileName("BlockAll");
	LeftWall->SetRelativeLocation(FVector(0.f, 0.f, -20.f));

	RightWall->SetupAttachment(FloorMesh);
	RightWall->SetCollisionProfileName("BlockAll");
	RightWall->SetRelativeLocation(FVector(0.f, 400.f, -20.f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WallMeshRef(
		TEXT("/Script/Engine.StaticMesh'/Game/_GameAssets/Meshes/Structure/Wall_400x200.Wall_400x200'"));
	if (IsValid(WallMeshRef.Object))
	{
		LeftWall->SetStaticMesh(WallMeshRef.Object);
		RightWall->SetStaticMesh(WallMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> WallMaterialRef(
		TEXT("/Script/Engine.Material'/Game/_GameAssets/Materials/Props/M_Brick.M_Brick'"));
	if (IsValid(WallMaterialRef.Object))
	{
		LeftWall->SetMaterial(0, WallMaterialRef.Object);
		RightWall->SetMaterial(0, WallMaterialRef.Object);
	}

}

void ACBasicFloor::BeginPlay()
{
	Super::BeginPlay();

}

void ACBasicFloor::CreateCeiling()
{
	CeilingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CeilingMesh"));
	CeilingMesh->SetupAttachment(FloorMesh);
	CeilingMesh->SetRelativeLocation(FVector(0.f, 0.f, 180.f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CeilingMeshRef(
		TEXT("/Script/Engine.StaticMesh'/Game/_GameAssets/Meshes/Structure/Floor_400x400.Floor_400x400'"));
	if (IsValid(CeilingMeshRef.Object))
	{
		CeilingMesh->SetStaticMesh(CeilingMeshRef.Object);
	}

	CeilingLightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CeilingLightMesh"));
	CeilingLightMesh->SetupAttachment(CeilingMesh);
	CeilingLightMesh->SetRelativeLocation({ 200.f, 200.f, -20.f });
	CeilingLightMesh->SetRelativeScale3D({ 0.3f, 0.3f, 0.3f });

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CeilingLightMeshRef(
		TEXT("/Script/Engine.StaticMesh'/Game/_GameAssets/Meshes/Props/SM_Ceiling_Light.SM_Ceiling_Light'"));
	if (IsValid(CeilingLightMeshRef.Object))
	{
		CeilingLightMesh->SetStaticMesh(CeilingLightMeshRef.Object);
	}

	CeilingLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("CeilingLight"));
	CeilingLight->SetupAttachment(CeilingLightMesh);
	CeilingLight->SetRelativeLocation({ 0.f, 0.f, -40.f });
	CeilingLight->SetIntensity(1000.f);
}


void ACBasicFloor::ActivateFloor()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	SpawnedHpUpItem = GetWorld()->SpawnActorDeferred<AActor>(BP_HpUpItem, GetActorTransform());
	if (IsValid(SpawnedHpUpItem))
	{
		SpawnedHpUpItem->AttachToComponent(FloorMesh, FAttachmentTransformRules::KeepRelativeTransform);
		SpawnedHpUpItem->SetActorRelativeLocation(FVector(200.f, FMath::FRandRange(100.f, 300.f),
			FMath::FRandRange(50.f, 150.f)));
	}
	SpawnedHpUpItem->FinishSpawning(GetActorTransform());
}

void ACBasicFloor::DeactivateFloor()
{
	SetActorHiddenInGame(true);
	if (IsValid(SpawnedHpUpItem))
	{
		SpawnedHpUpItem->Destroy();
	}

	FTimerHandle DestroyTimerHandle;
	constexpr float DestroyTime = 0.1f;

	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			SetActorEnableCollision(false);
			GetWorld()->GetTimerManager().ClearTimer(DestroyTimerHandle);
		}), DestroyTime, false);
}

