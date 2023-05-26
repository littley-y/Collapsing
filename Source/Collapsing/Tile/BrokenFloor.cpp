// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/BrokenFloor.h"
#include "Item/CHpUpItem.h"

ABrokenFloor::ABrokenFloor()
{
	HpUpItem->DestroyComponent();
}
