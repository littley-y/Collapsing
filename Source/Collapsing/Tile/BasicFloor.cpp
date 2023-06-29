// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicFloor.h"
#include "Character/RunCharacter.h"
#include "Item/CHpUpItem.h"

ABasicFloor::ABasicFloor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	check(SceneComponent)

	RootComponent = SceneComponent;
	CreateFloorAndCeiling();
	CreateWalls();

	// Obstacle

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

void ABasicFloor::CreateFloorAndCeiling()
{
	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(SceneComponent);
	FloorMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	FloorMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));

	CeilingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CeilingMesh"));
	CeilingMesh->SetupAttachment(FloorMesh);
	CeilingMesh->SetRelativeLocation(FVector(0.f, 0.f, 180.f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FloorMeshRef(TEXT("/Game/_GameAssets/Meshes/Floor_400x400"));
	if (IsValid(FloorMeshRef.Object))
	{
		FloorMesh->SetStaticMesh(FloorMeshRef.Object);
		CeilingMesh->SetStaticMesh(FloorMeshRef.Object);
	}
}

void ABasicFloor::CreateWalls()
{
	Walls.SetNum(2);
	for (int32 Ix = 0; Ix != Walls.Num(); ++Ix)
	{
		SetWall(Walls[Ix], Ix);
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

void ABasicFloor::BeginPlay()
{
	Super::BeginPlay();

	UCHpUpItem* HpUpItem = NewObject<UCHpUpItem>(this, UCHpUpItem::StaticClass(), TEXT("HpUpItem"));
	if (IsValid(HpUpItem))
	{
		HpUpItem->RegisterComponent();
		HpUpItem->AttachToComponent(FloorMesh, FAttachmentTransformRules::KeepRelativeTransform);
		HpUpItem->SetRelativeLocation(FVector(200.f, FMath::FRandRange(100.f, 300.f), FMath::FRandRange(50.f, 150.f)));
		HpUpItem->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	}
}

void ABasicFloor::OnWallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	if (IsValid(RunCharacter))
	{
		const float Dot = FVector::DotProduct(Hit.Normal, RunCharacter->GetActorForwardVector());
		if (Dot > 0.99f && Dot < 1.01f)
		{
			RunCharacter->Death();
		}
	}
}
