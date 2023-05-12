// Fill out your copyright notice in the Description page of Project Settings.

#include "TileGenerator.h"
#include <Utils/AssetFinder.hpp>
#include "GeometryCollection/GeometryCollectionObject.h"
#include "Field/FieldSystemActor.h"

void FTileGeneratorTransform::UpdateVectorXY(const float InValue)
{
	const float CurrentDegrees = FMath::FindDeltaAngleDegrees(Rotator.Yaw, 0.f);
	if (FMath::IsNearlyEqual(CurrentDegrees, 0.f))
	{
		Vector.X += InValue;
	}
	else if (FMath::IsNearlyEqual(CurrentDegrees, -90.f))
	{
		Vector.Y += InValue;
	}
	else if (FMath::IsNearlyEqual(FMath::Abs(CurrentDegrees), 180.f))
	{
		Vector.X -= InValue;
	}
	else if (FMath::IsNearlyEqual(CurrentDegrees, 90.f))
	{
		Vector.Y -= InValue;
	}

	if (FMath::IsNearlyEqual(FMath::Abs(Rotator.Yaw), 360.f))
	{
		Rotator.Yaw = 0.f;
	}
}

void FTileGeneratorTransform::UpdateVectorZ(const float InValue)
{

}

UTileGenerator::UTileGenerator()
{
	LoadBPFloor(TEXT("/Game/Collapsing/Tile/BP_CBasicFloor"), '0');
	LoadBPFloor(TEXT("/Game/Collapsing/Tile/BP_CLeftCornerFloor"), 'L');
	LoadBPFloor(TEXT("/Game/Collapsing/Tile/BP_CRightCornerFloor"), 'R');
	LoadBPFloor(TEXT("/Game/Collapsing/Tile/BP_CSpeedFloor"), 'S');
	LoadBPFloor(TEXT("/Game/Collapsing/Tile/BP_CBrokenFloor"), 'B');

	GeneratedFloorArray.SetNum(MaxTileNum + 1);
	CurrentMapIndex = 0;

	// CPP 클래스 만들어서 그 아래 GeometryCollectionComponent 두고, BP로 상속받아 만든다음 내가 만든 Geometry로 지정해주기
	Test1 = MyFunction::AssetClassFinder<AActor>(TEXT("/Game/Collapsing/Tile/BP_CBasicFloor_GeometryCollection.BP_CBasicFloor_GeometryCollection_C"));
	Test2 = MyFunction::AssetClassFinder<AActor>(TEXT("/Game/Collapsing/Tile/FS_TEST2.FS_TEST2_C"));
}

void UTileGenerator::LoadBPFloor(const FString& BPPath, uint8 KeyChar)
{
	ConstructorHelpers::FClassFinder<AActor> BPFloor(*BPPath);
	if (BPFloor.Succeeded())
	{
		const int32 Position = BPPath.Find(TEXT("/"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		BPFloorArray.Emplace(KeyChar, BPFloor.Class);
		UE_LOG(LogTemp, Warning, TEXT("%s Loaded"), *BPPath.RightChop(Position + 1));
	}
}

void UTileGenerator::SpawnFloorTile()
{
	const TCHAR& MapChar = MapString[CurrentMapIndex % MapString.Len()];
	const int32 ArrayIndex = CurrentMapIndex % MaxTileNum;
	UE_LOG(LogTemp, Warning, TEXT("Current Val : %c"), MapChar)

	if (CurrentMapIndex >= MaxTileNum && IsValid(GeneratedFloorArray[ArrayIndex]))
	{
		FVector CurrLocation = GeneratedFloorArray[ArrayIndex]->GetActorLocation();
		GetWorld()->SpawnActor<AActor>(Test1, CurrLocation, FRotator::ZeroRotator);
		GetWorld()->SpawnActor<AActor>(Test2, CurrLocation, FRotator::ZeroRotator);
		GeneratedFloorArray[ArrayIndex]->Destroy();
	}

	GeneratedFloorArray[ArrayIndex] = GetWorld()->SpawnActor<AActor>(BPFloorArray[MapChar], TileGenTrans.Vector,
	                                                                 TileGenTrans.Rotator);
	if (MapChar == 'L')
	{
		TileGenTrans.Rotator.Yaw -= 90.f;
	}
	else if (MapChar == 'R')
	{
		TileGenTrans.UpdateVectorXY(TileSize);
		TileGenTrans.Rotator.Yaw += 90.f;
		TileGenTrans.UpdateVectorXY(TileSize);
	}
	else
	{
		TileGenTrans.UpdateVectorXY(TileSize);
	}

	CurrentMapIndex++;

	UE_LOG(LogTemp, Warning, TEXT("Floor Gen Location : %s, Yaw : %f"), *TileGenTrans.Vector.ToString(), TileGenTrans.Rotator.Yaw)
}

void UTileGenerator::InitMaps()
{
	for (int32 It = 0; It != InitTileNum; ++It)
	{
		SpawnFloorTile();
	}
	UE_LOG(LogTemp, Warning, TEXT("Init Tiles Generated : %d"), MaxTileNum)
}
