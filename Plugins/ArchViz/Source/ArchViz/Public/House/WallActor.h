// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "House/HouseComponent.h"
#include "WallActor.generated.h"

class ADoorActor;
/**
 * 
 */
UCLASS()
class ARCHVIZ_API AWallActor : public AHouseComponent
{
	GENERATED_BODY()

public:

	AWallActor();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 NumberOfWallSegments;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 LengthOfSegment;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* WallSegmentMesh;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* DoorHallSegment;

	UPROPERTY()
	UMaterialInterface* Material;

	UPROPERTY()
	TArray<UStaticMeshComponent*> ArrayOfWallSegments;

	TArray<int32> ArrayOfDoorIndices;

	TMap<int32, ADoorActor*> IndexToDoorMapping;

	void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void UpdateWall();



	UFUNCTION()
	void SetLength(float Length);

	virtual void HighLightBorder() override;
	virtual void UnHighLightBorder() override;


	UFUNCTION()
	 void SetMaterial(FMaterialInfo MaterialInfo);

	void AttachDoorToComponent(UStaticMeshComponent* Component, ADoorActor* Door);
	void DetachDoorFromComponent(UStaticMeshComponent* Component);

	void RemoveAllDoorsFromWall();

};
