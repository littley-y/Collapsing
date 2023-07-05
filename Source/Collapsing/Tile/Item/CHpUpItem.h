// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CHpUpItem.generated.h"

UCLASS()
class COLLAPSING_API ACHpUpItem : public AActor
{
	GENERATED_BODY()
	
public:
	ACHpUpItem();

	UFUNCTION()
	void OnPlayerItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                         int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FORCEINLINE UStaticMeshComponent* GetItem() const { return ItemMeshComponent; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	TObjectPtr<UStaticMeshComponent> ItemMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<USoundBase> PickSound;



};
