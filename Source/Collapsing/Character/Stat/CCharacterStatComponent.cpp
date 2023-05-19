// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Stat/CCharacterStatComponent.h"

// Sets default values for this component's properties
UCCharacterStatComponent::UCCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
