// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicFloor.generated.h"

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
	TArray<UStaticMeshComponent*> WallArray;

	// Sets default values for this actor's properties
	ABasicFloor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnWallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	               FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CreateWall(UStaticMeshComponent* Obj) const;

	void CreateFloor(UStaticMeshComponent* Obj) const;
};

