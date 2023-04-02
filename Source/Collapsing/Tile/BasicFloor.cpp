// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicFloor.h"
#include "Collapsing/Character/RunCharacter.h"
#include "Engine/StaticMeshSocket.h"

// Sets default values
ABasicFloor::ABasicFloor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	CreateFloor();
	CreateWallArray();
}

// Called when the game starts or when spawned
void ABasicFloor::BeginPlay()
{
	Super::BeginPlay();
}

void ABasicFloor::CreateWallArray()
{
	WallArray.SetNum(2);
	for (int32 Ix = 0; Ix != WallArray.Num(); ++Ix)
	{
		CreateWall(WallArray[Ix], Ix);
		if (Ix == 1)
		{
			WallArray[Ix]->SetRelativeLocation(FVector(0.f, 400.f, -20.f));
		}
	}
	LoadObject<UClass>(nullptr, TEXT("BP_LeftCornerFloor"));
}

void ABasicFloor::CreateWall(UStaticMeshComponent*& Wall, const int32& Ix)
{
	FString MeshName(TEXT("WallMesh"));
	MeshName += FString::FromInt(Ix);
	Wall = CreateDefaultSubobject<UStaticMeshComponent>(*MeshName);

	Wall->SetupAttachment(FloorMesh);
	Wall->SetCollisionProfileName("BlockAll");
	Wall->SetRelativeLocation(FVector(0.f, 0.f, -20.f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/_GameAssets/Meshes/Wall_400x200"));
	if (MeshAsset.Succeeded())
	{
		Wall->SetStaticMesh(MeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> WoodMaterialAsset(
		TEXT("/Game/_GameAssets/Meshes/Materials/M_Wood_Floor_Walnut_Polished"));

	if (WoodMaterialAsset.Succeeded())
	{
		Wall->SetMaterial(0, WoodMaterialAsset.Object);
	}
}

void ABasicFloor::CreateFloor()
{
	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(SceneComponent);
	FloorMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/_GameAssets/Meshes/Floor_400x400"));
	if (MeshAsset.Succeeded())
	{
		FloorMesh->SetStaticMesh(MeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> WoodMaterialAsset(
		TEXT("/Game/_GameAssets/Meshes/Materials/M_Wood_Floor_Walnut_Polished"));
	if (WoodMaterialAsset.Succeeded())
	{
		FloorMesh->SetMaterial(0, WoodMaterialAsset.Object);
	}
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
		if (FMath::IsNearlyEqual(Dot, 1.f, 0.1f))
		{
			RunCharacter->Death();
		}
	}
}
