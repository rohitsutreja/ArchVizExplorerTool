// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "House/HouseComponent.h"
#include "ProceduralMeshComponent.h"
#include "FloorActor.generated.h"


/**
 * 
 */
UCLASS()
class ARCHVIZ_API AFloorActor : public AHouseComponent
{
	GENERATED_BODY()


public:
	AFloorActor();

	void GenerateFloor();

	UPROPERTY(EditAnywhere)
	int32 Height;

	UPROPERTY(EditAnywhere)
	int32 Width;

	UPROPERTY(EditAnywhere)
	int32 Length;

	UPROPERTY()
	UProceduralMeshComponent* ProcMesh;

	UPROPERTY()
	UMaterialInterface* Material;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void HighLightBorder() override;
	virtual void UnHighLightBorder() override;

	UFUNCTION()

	void SetMaterial(FMaterialInfo MaterialInfo);

	UFUNCTION()
	void UpdateDimensions(float X);

	virtual void BeginPlay() override;

};
