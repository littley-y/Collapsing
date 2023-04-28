#pragma once

namespace MyFunction
{
	template <typename T>
	T* AssetObjectFinder(const FString& FilePath)
	{
		ConstructorHelpers::FObjectFinder<T> TargetAsset(*FilePath);
		if (TargetAsset.Succeeded())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s Loaded"), *FilePath);
			return TargetAsset.Object;
		}
		return nullptr;
	}

	template <typename T>
	TSubclassOf<T> AssetClassFinder(const FString& FilePath)
	{
		ConstructorHelpers::FClassFinder<T> TargetAsset(*FilePath);
		if (TargetAsset.Succeeded())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s Loaded"), *FilePath);
			return TargetAsset.Class;
		}
		return nullptr;
	}
}
