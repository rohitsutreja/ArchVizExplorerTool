// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MaterialDataAsset.generated.h"

/**
 * 
 */


USTRUCT(BlueprintType)
struct FMaterialData
{
	GENERATED_BODY()

	UPROPERTY()
	UTexture* Thumbnail;

	UPROPERTY()
	UMaterialInterface* Material;

	UPROPERTY()
	FString Title;
};



UCLASS()
class ARCHVIZ_API UMaterialDataAsset : public UDataAsset
{
	GENERATED_BODY()



public:
	UPROPERTY(EditAnywhere)
	TArray<FMaterialData> ArrayOfMaterials;
};
