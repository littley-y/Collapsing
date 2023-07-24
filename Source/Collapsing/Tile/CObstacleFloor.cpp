// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile/CObstacleFloor.h"
#include "Interface/CCharacterInteractionInterface.h"

ACObstacleFloor::ACObstacleFloor()
{
	PrimaryActorTick.bCanEverTick = false;

	Obstacle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Obstacle"));
	Obstacle->SetupAttachment(RootComponent);
	Obstacle->OnComponentHit.AddDynamic(this, &ACObstacleFloor::OnObstacleHit);
	Obstacle->SetCollisionProfileName(TEXT("BlockNotCamera"));

	WallObstacleLocations.Add({200.f, 100.f, 150.f});
	WallObstacleLocations.Add({200.f, 700.f, 150.f});

	FString ObstaclePaths[] = {
		"/Script/Engine.StaticMesh'/Game/_GameAssets/Meshes/Props/SM_Bed_Drawer.SM_Bed_Drawer'",
		"/Script/Engine.StaticMesh'/Game/_GameAssets/Meshes/Props/SM_Bed_table.SM_Bed_table'",
		"/Script/Engine.StaticMesh'/Game/_GameAssets/Meshes/Props/SM_Footrest.SM_Footrest'",
		"/Script/Engine.StaticMesh'/Game/_GameAssets/Meshes/Props/SM_Trash_Can.SM_Trash_Can'",
		"/Script/Engine.StaticMesh'/Game/_GameAssets/Meshes/Props/SM_ClosetR3.SM_ClosetR3'",
		"/Script/Engine.StaticMesh'/Game/_GameAssets/Meshes/Props/SM_Sofa2.SM_Sofa2'",
		"/Script/Engine.StaticMesh'/Game/_GameAssets/Meshes/Props/SM_LaundryShelf.SM_LaundryShelf'"
	};

	for (auto ObstaclePath : ObstaclePaths)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> ObstacleRef(*ObstaclePath);
		if (IsValid(ObstacleRef.Object))
		{
			ObstacleMeshes.Add(ObstacleRef.Object);
		}
	}
}

void ACObstacleFloor::OnObstacleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ICCharacterInteractionInterface* RunCharacter = Cast<ICCharacterInteractionInterface>(OtherActor);
	if (RunCharacter != nullptr)
	{
		const float Dot = FVector::DotProduct(Hit.Normal, OtherActor->GetActorForwardVector());
		if (!FMath::IsNearlyEqual(0.f, Dot, 0.1f))
		{
			RunCharacter->HitBySomething();
		}
	}
}

void ACObstacleFloor::ActivateFloor()
{
	Super::ActivateFloor();

	Obstacle->Activate();
}

void ACObstacleFloor::DeactivateFloor()
{
	Super::DeactivateFloor();

	Obstacle->Deactivate();
}

void ACObstacleFloor::BeginPlay()
{
	Super::BeginPlay();

	const int32 FloorObstacleType = FMath::RandRange(Bed_Drawer, LaundryShelf);
	Obstacle->SetStaticMesh(ObstacleMeshes[FloorObstacleType]);

	const FVector SofaLocation = {600.f, 400.f, 0.f};
	const FVector ClosetLocation = {600.f, FMath::RandRange(1, 3) * 200.f, 0.f};
	const FVector FloorObstacleLocation = {600.f, FMath::FRandRange(200.f, 600.f), 0.f};

	switch (FloorObstacleType)
	{
	default: break;
	case Bed_Drawer:
		Obstacle->SetRelativeLocation(FloorObstacleLocation);
		Obstacle->SetRelativeRotation({0.f, -90.f, 0.f});
		Obstacle->SetRelativeScale3D({3.f, 2.f, 2.f});
		break;
	case Bed_Table:
		Obstacle->SetRelativeLocation(FloorObstacleLocation);
		Obstacle->SetRelativeRotation({0.f, -90.f, 0.f});
		Obstacle->SetRelativeScale3D({3.f, 2.f, 2.f});
		break;
	case Footrest:
		Obstacle->SetRelativeLocation(FloorObstacleLocation);
		Obstacle->SetRelativeRotation({0.f, -90.f, 0.f});
		Obstacle->SetRelativeScale3D({3.f, 2.f, 2.f});
		break;
	case Trash_Can:
		Obstacle->SetRelativeLocation(FloorObstacleLocation);
		Obstacle->SetRelativeRotation({0.f, -90.f, 0.f});
		Obstacle->SetRelativeScale3D({3.f, 2.f, 1.f});
		break;
	case Closet:
		Obstacle->SetRelativeLocation(ClosetLocation);
		Obstacle->SetRelativeRotation({0.f, 90.f, 0.f});
		Obstacle->SetRelativeScale3D({2.f, 1.f, 1.f});
		break;
	case Sofa:
		Obstacle->SetRelativeLocation(SofaLocation);
		Obstacle->SetRelativeRotation({0.f, -90.f, 0.f});
		Obstacle->SetRelativeScale3D({1.8f, 1.f, 1.1f});
		break;
	case LaundryShelf:
		const bool IsLocatedLeftWall = FMath::RandBool() ? true : false;
		Obstacle->SetRelativeLocation(WallObstacleLocations[IsLocatedLeftWall]);
		Obstacle->SetRelativeRotation({0.f, 0.f, IsLocatedLeftWall ? -60.f : 60.f});
		Obstacle->SetRelativeScale3D({2.f, 2.f, 2.f});
		break;
	}
}
