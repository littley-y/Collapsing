// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicFloor.generated.h"

class UTileGenerator;

UCLASS()
class COLLAPSING_API ABasicFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> FloorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TArray<TObjectPtr<UStaticMeshComponent>> WallArray;

	ABasicFloor();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnWallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	               FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnGenerateBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                               const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UBoxComponent> GenerateTileZone;

	virtual void BeginPlay() override;

	void SetWallArray();

	void SetWall(UStaticMeshComponent* Wall) const;

	void CreateFloor();

	void SetGenerateTileZone();
};

