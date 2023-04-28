// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicFloor.h"
#include "Game/CollapsingGameMode.h"
#include "Character/RunCharacter.h"
#include "Utils/AssetFinder.hpp"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ABasicFloor::ABasicFloor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	check(SceneComponent)

	RootComponent = SceneComponent;

	ABasicFloor::CreateFloor();

	SetWallArray();

	SetGenerateTileZone();
}

void ABasicFloor::BeginPlay()
{
	Super::BeginPlay();
}

void ABasicFloor::SetWallArray()
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

	static UStaticMesh* WallMeshAsset = MyFunction::AssetObjectFinder<UStaticMesh>(TEXT("/Game/_GameAssets/Meshes/Wall_400x200"));
	if (IsValid(WallMeshAsset))
	{
		Wall->SetStaticMesh(WallMeshAsset);
	}

	static UMaterial* WoodMaterialAsset = MyFunction::AssetObjectFinder<UMaterial>(TEXT("/Game/_GameAssets/Meshes/Materials/M_Wood_Floor_Walnut_Polished"));
	if (IsValid(WoodMaterialAsset))
	{
		Wall->SetMaterial(0, WoodMaterialAsset);
	}

	if (Ix == 1)
	{
		WallArray[Ix]->SetRelativeLocation(FVector(0.f, 400.f, -20.f));
	}
}

void ABasicFloor::CreateFloor()
{
	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(SceneComponent);
	FloorMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	
	static UStaticMesh* FloorMeshAsset = MyFunction::AssetObjectFinder<UStaticMesh>(TEXT("/Game/_GameAssets/Meshes/Floor_400x400"));
	if (IsValid(FloorMeshAsset))
	{
		FloorMesh->SetStaticMesh(FloorMeshAsset);
	}

	static UMaterial* WoodMaterialAsset = MyFunction::AssetObjectFinder<UMaterial>(TEXT("/Game/_GameAssets/Meshes/Materials/M_Wood_Floor_Walnut_Polished"));
	if (IsValid(WoodMaterialAsset))
	{
		FloorMesh->SetMaterial(0, WoodMaterialAsset);
	}
}

void ABasicFloor::SetGenerateTileZone()
{
	GenerateTileZone = CreateDefaultSubobject<UBoxComponent>(TEXT("GenerateTileZone"));
	if (IsValid(GenerateTileZone))
	{
		GenerateTileZone->SetupAttachment(SceneComponent);
		GenerateTileZone->SetBoxExtent(FVector(200.f, 200.f, 30.f));
		GenerateTileZone->SetRelativeLocation(FVector(400.f, 200.f, 200.f));
		GenerateTileZone->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

		GenerateTileZone->SetGenerateOverlapEvents(true);
		GenerateTileZone->OnComponentBeginOverlap.AddDynamic(this, &ABasicFloor::OnGenerateBoxBeginOverlap);
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

void ABasicFloor::OnGenerateBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	if (IsValid(RunCharacter) && IsValid(OtherComp))
	{
		// 델리게이트로 불러서 최적화 할 수 있는 방법 없으려나...
		const ACollapsingGameMode* RunGameMode = Cast<ACollapsingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (IsValid(RunGameMode))
		{
			RunGameMode->GenerateTile();
		}
	}
}
