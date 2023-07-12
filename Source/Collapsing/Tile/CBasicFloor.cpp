// Fill out your copyright notice in the Description page of Project Settings.

#include "CBasicFloor.h"

#include "Item/CHpUpItem.h"

ACBasicFloor::ACBasicFloor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	check(SceneComponent);

	RootComponent = SceneComponent;
	CreateFloorAndCeiling();
	CreateWalls();
}

void ACBasicFloor::CreateFloorAndCeiling()
{
	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(SceneComponent);
	FloorMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	FloorMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));

	CeilingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CeilingMesh"));
	CeilingMesh->SetupAttachment(FloorMesh);
	CeilingMesh->SetRelativeLocation(FVector(0.f, 0.f, 180.f));

	static ConstructorHelpers::FClassFinder<AActor> HpUpItemRef(TEXT("/Game/Collapsing/Tile/Item/BP_CHpUpItem"));
	if (IsValid(HpUpItemRef.Class))
	{
		BP_HpUpItem = HpUpItemRef.Class;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FloorMeshRef(TEXT("/Game/_GameAssets/Meshes/Floor_400x400"));
	if (IsValid(FloorMeshRef.Object))
	{
		FloorMesh->SetStaticMesh(FloorMeshRef.Object);
		CeilingMesh->SetStaticMesh(FloorMeshRef.Object);
	}
}

void ACBasicFloor::CreateWalls()
{
	Walls.SetNum(2);
	for (int32 Ix = 0; Ix != Walls.Num(); ++Ix)
	{
		SetWall(Walls[Ix], Ix);
	}

	TArray<FName> SocketNames = Walls[0]->GetAllSocketNames();
	for (auto& SocketName : SocketNames)
	{
		FString ObstacleName = FString::Printf(TEXT("Left%s"), *SocketName.ToString());
		UStaticMeshComponent* CurrObstacle = CreateDefaultSubobject<UStaticMeshComponent>(*ObstacleName);
		CurrObstacle->SetupAttachment(Walls[0], SocketName);
		CurrObstacle->SetCollisionProfileName(TEXT("BlockNotCamera"));
		Obstacles.Add(CurrObstacle);
	}
}

void ACBasicFloor::ActiveFloor()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	SpawnedHpUpItem = GetWorld()->SpawnActorDeferred<AActor>(BP_HpUpItem, GetActorTransform());
	if (IsValid(SpawnedHpUpItem))
	{
		SpawnedHpUpItem->AttachToComponent(FloorMesh, FAttachmentTransformRules::KeepRelativeTransform);
		SpawnedHpUpItem->SetActorRelativeLocation(FVector(200.f, FMath::FRandRange(100.f, 300.f), FMath::FRandRange(50.f, 150.f)));
	}
	SpawnedHpUpItem->FinishSpawning(GetActorTransform());
}

void ACBasicFloor::DeactiveFloor()
{
	SetActorHiddenInGame(true);
	if (IsValid(SpawnedHpUpItem))
	{
		SpawnedHpUpItem->Destroy();
	}

	FTimerHandle DestroyTimerHandle;
	const float DestroyTime = 0.1f;

	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			SetActorEnableCollision(false);
			GetWorld()->GetTimerManager().ClearTimer(DestroyTimerHandle);
		}), DestroyTime, false);
}

void ACBasicFloor::SetWall(TObjectPtr<UStaticMeshComponent>& Wall, const int8 Ix)
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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WallMeshRef(TEXT("/Game/_GameAssets/Meshes/Wall_400x200"));
	if (IsValid(WallMeshRef.Object))
	{
		Wall->SetStaticMesh(WallMeshRef.Object);
	}

	if (Ix == 1)
	{
		Walls[Ix]->SetRelativeLocation(FVector(0.f, 400.f, -20.f));
	}
}
