// Fill out your copyright notice in the Description page of Project Settings.


#include "BrokenFloor.h"
#include "Collapsing/CollapsingGameModeBase.h"
#include "Collapsing/Character/RunCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABrokenFloor::ABrokenFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	check(IsValid(SceneComponent))

	RootComponent = SceneComponent;

	SetGenerateTileZone();
}

void ABrokenFloor::OnGenerateBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	if (IsValid(RunCharacter) && IsValid(OtherComp))
	{
		// 델리게이트로 불러서 최적화 할 수 있는 방법 없으려나...
		const ACollapsingGameModeBase* RunGameMode = Cast<ACollapsingGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (IsValid(RunGameMode))
		{
			RunGameMode->GenerateTile();
		}
	}
}

void ABrokenFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABrokenFloor::SetGenerateTileZone()
{
	GenerateTileZone = CreateDefaultSubobject<UBoxComponent>(TEXT("GenerateTileZone"));
	if (IsValid(GenerateTileZone))
	{
		GenerateTileZone->SetupAttachment(SceneComponent);
		GenerateTileZone->SetBoxExtent(FVector(200.f, 200.f, 30.f));
		GenerateTileZone->SetRelativeLocation(FVector(400.f, 200.f, 200.f));
		GenerateTileZone->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

		GenerateTileZone->SetGenerateOverlapEvents(true);
		GenerateTileZone->OnComponentBeginOverlap.AddDynamic(this, &ABrokenFloor::OnGenerateBoxBeginOverlap);
	}
}
