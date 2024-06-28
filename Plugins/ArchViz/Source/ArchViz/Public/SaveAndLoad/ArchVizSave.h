// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "House/DoorActor.h"
#include "Interior/InteriorActor.h"
#include "Roads/RoadSplineActor.h"
#include "ArchVizSave.generated.h"

/**
 * 
 */

USTRUCT()
struct FRoad{

	GENERATED_BODY()

	UPROPERTY()
	int32 ID;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	TArray<FVector> SplinePoints;

	UPROPERTY()
	ERoadType Type;

	UPROPERTY()
	float Width;

	UPROPERTY()
	UMaterialInterface* Material;

	UPROPERTY()
	int32 ParentActorId;
};

USTRUCT()
struct FWall{

	GENERATED_BODY()

	UPROPERTY()
	int32 ID;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	int32 NumberOfWallSegments;

	UPROPERTY()
	UMaterialInterface* Material;

	UPROPERTY()
	int32 ParentActorId;
};

USTRUCT()
struct FInterior {
	GENERATED_BODY()

	UPROPERTY()
	int32 ID;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	UStaticMesh* StaticMesh;

	UPROPERTY()
	EItemCategory Category;

	UPROPERTY()
	int32 ParentActorId;
};

USTRUCT()
struct FFloor {
	GENERATED_BODY()

	UPROPERTY()
	int32 ID;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FVector Dimensions;

	UPROPERTY()
	UMaterialInterface* Material;

	UPROPERTY()
	int32 ParentActorId;
};


USTRUCT()
struct FDoor {
	GENERATED_BODY()

	UPROPERTY()
	int32 ID;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	UMaterialInterface* DoorMaterial;

	UPROPERTY()
	UMaterialInterface* FrameMaterial;

	UPROPERTY()
	bool bIsOpen;

	UPROPERTY()
	int32 ParentActorId;

	UPROPERTY()
	int32 ParentComponentIndex;
};



USTRUCT()
struct FWindow {
	GENERATED_BODY()

	UPROPERTY()
	int32 ID;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	UMaterialInterface* WindowMaterial;

	UPROPERTY()
	int32 ParentActorId;

	UPROPERTY()
	int32 ParentComponentIndex;
};

UCLASS()
class ARCHVIZ_API UArchVizSave : public USaveGame
{
	GENERATED_BODY()

	public:

	UPROPERTY()
	TArray<FRoad> RoadActorArray;

	UPROPERTY()
	TArray<FWall> WallActorArray;

	UPROPERTY()
	TArray<FInterior> InteriorActorArray;

	UPROPERTY()
	TArray<FFloor> FloorActorArray;

	UPROPERTY()
	TArray<FDoor> DoorActorArray;

	UPROPERTY()
	TArray<FWindow> WindowActorArray;

	UPROPERTY()
	TArray<FString> SaveMetaDataArray;
};
