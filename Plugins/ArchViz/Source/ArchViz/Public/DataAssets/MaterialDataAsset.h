// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MaterialDataAsset.generated.h"

/**
 * 
 */


USTRUCT(BlueprintType)
struct FMaterialInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UTexture* Thumbnail;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;
};



UCLASS()
class ARCHVIZ_API UMaterialDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FMaterialInfo> ArrayOfWallMaterials;

	UPROPERTY(EditAnywhere)
	TArray<FMaterialInfo> ArrayOfFloorMaterials;

	UPROPERTY(EditAnywhere)
	TArray<FMaterialInfo> ArrayOfRoadMaterials;

	UPROPERTY(EditAnywhere)
	TArray<FMaterialInfo> ArrayOfDoorMaterials;

	UPROPERTY(EditAnywhere)
	TArray<FMaterialInfo> ArrayOFDoorFrameMaterials;

	UPROPERTY(EditAnywhere)
	TArray<FMaterialInfo> ArrayOFCeilingMaterials;
};
