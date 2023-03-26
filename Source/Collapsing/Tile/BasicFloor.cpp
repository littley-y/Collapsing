// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicFloor.h"

#include <string>

#include "Collapsing/Character/RunCharacter.h"
#include "Engine/StaticMeshSocket.h"

// Sets default values
ABasicFloor::ABasicFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(SceneComponent);

	LeftWallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftWallMesh"));
	LeftWallMesh->SetupAttachment(FloorMesh);
	LeftWallMesh->SetCollisionProfileName("BlockAll");

	RightWallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightWallMesh"));
	RightWallMesh->SetupAttachment(FloorMesh);
	RightWallMesh->SetCollisionProfileName("BlockAll");

	SetBasicMash();
    ABasicFloor::SetMeshLocation();

	LeftWallMesh->OnComponentHit.AddDynamic(this, &ABasicFloor::OnWallHit);
	RightWallMesh->OnComponentHit.AddDynamic(this, &ABasicFloor::OnWallHit);
}

void ABasicFloor::SetBasicMash() const
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FloorMeshAsset(
		TEXT("/Game/_GameAssets/Meshes/Floor_400x400"));
	if (FloorMeshAsset.Succeeded())
	{
		FloorMesh->SetStaticMesh(FloorMeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WallMeshAsset(
		TEXT("/Game/_GameAssets/Meshes/Wall_400x200"));
	if (WallMeshAsset.Succeeded())
	{
		LeftWallMesh->SetStaticMesh(WallMeshAsset.Object);
		RightWallMesh->SetStaticMesh(WallMeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> WoodMaterialAsset(
		TEXT("/Game/_GameAssets/Meshes/Materials/M_Wood_Floor_Walnut_Polished"));
	if (WoodMaterialAsset.Succeeded())
	{
		FloorMesh->SetMaterial(0, WoodMaterialAsset.Object);
		LeftWallMesh->SetMaterial(0, WoodMaterialAsset.Object);
		RightWallMesh->SetMaterial(0, WoodMaterialAsset.Object);
	}
}

void ABasicFloor::SetMeshLocation() const
{
	FloorMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	LeftWallMesh->SetRelativeLocation(FVector(0.f, 0.f, -20.f));
	RightWallMesh->SetRelativeLocation(FVector(0.f, 400.f, -20.f));
}

// Called when the game starts or when spawned
void ABasicFloor::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void ABasicFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ABasicFloor::OnWallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                             FVector NormalImpulse, const FHitResult& Hit)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	if (RunCharacter != nullptr)
	{
		const float Dot = FVector::DotProduct(Hit.Normal, RunCharacter->GetActorForwardVector());
		if (Dot > 0.99f && Dot < 1.01f)
		{
			RunCharacter->Death();
		}
	}
}