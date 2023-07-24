// Fill out your copyright notice in the Description page of Project Settings.

#include "CBasicFloor.h"
#include "Components/PointLightComponent.h"
#include "Interface/CCharacterInteractionInterface.h"
#include "Item/CHpUpItem.h"

ACBasicFloor::ACBasicFloor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	check(SceneComponent);

	RootComponent = SceneComponent;
	CreateFloor();
	CreateWalls();
	CreateCeiling();
}

void ACBasicFloor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
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
		TEXT("/Script/Engine.StaticMesh'/Game/_GameAssets/Meshes/Structures/SM_Floor.SM_Floor'"));
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
		TEXT("/Script/Engine.StaticMesh'/Game/_GameAssets/Meshes/Structures/SM_Wall.SM_Wall'"));
	if (IsValid(WallMeshRef.Object))
	{
		LeftWall->SetStaticMesh(WallMeshRef.Object);
		RightWall->SetStaticMesh(WallMeshRef.Object);
	}

	LeftWall->OnComponentHit.AddDynamic(this, &ACBasicFloor::OnWallHit);
	RightWall->OnComponentHit.AddDynamic(this, &ACBasicFloor::OnWallHit);
}

void ACBasicFloor::CreateCeiling()
{
	CeilingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CeilingMesh"));
	CeilingMesh->SetupAttachment(FloorMesh);
	CeilingMesh->SetRelativeLocation(FVector(0.f, 0.f, 180.f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CeilingMeshRef(
		TEXT("/Script/Engine.StaticMesh'/Game/_GameAssets/Meshes/Structures/SM_Ceiling.SM_Ceiling'"));
	if (IsValid(CeilingMeshRef.Object))
	{
		CeilingMesh->SetStaticMesh(CeilingMeshRef.Object);
	}

	CeilingLightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CeilingLightMesh"));
	CeilingLightMesh->SetupAttachment(CeilingMesh);
	CeilingLightMesh->SetRelativeLocation({200.f, 200.f, -20.f});
	CeilingLightMesh->SetRelativeScale3D({0.3f, 0.3f, 0.3f});
	CeilingLightMesh->SetCollisionProfileName(TEXT("BlockNotCamera"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CeilingLightMeshRef(
		TEXT("/Script/Engine.StaticMesh'/Game/_GameAssets/Meshes/Props/SM_Ceiling_Light.SM_Ceiling_Light'"));
	if (IsValid(CeilingLightMeshRef.Object))
	{
		CeilingLightMesh->SetStaticMesh(CeilingLightMeshRef.Object);
	}

	CeilingLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("CeilingLight"));
	CeilingLight->SetupAttachment(CeilingLightMesh);
	CeilingLight->SetRelativeLocation({0.f, 0.f, -50.f});

	CeilingLight->SetIntensity(10000.f);
	CeilingLight->SetUseTemperature(true);
	CeilingLight->SetTemperature(5500.f);
	CeilingLight->SetCastShadows(true);
}


void ACBasicFloor::ActivateFloor()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);

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
	SetActorTickEnabled(false);
	SetActorEnableCollision(false);
	if (IsValid(SpawnedHpUpItem))
	{
		SpawnedHpUpItem->Destroy();
	}
}

void ACBasicFloor::OnWallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                             FVector NormalImpulse, const FHitResult& Hit)
{
	ICCharacterInteractionInterface* RunCharacter = Cast<ICCharacterInteractionInterface>(OtherActor);
	if (RunCharacter != nullptr)
	{
		const float Dot = FVector::DotProduct(Hit.Normal, OtherActor->GetActorForwardVector());
		if (Dot > 0.99f && Dot < 1.01f)
		{
			RunCharacter->HitBySomething();
		}
	}
}
