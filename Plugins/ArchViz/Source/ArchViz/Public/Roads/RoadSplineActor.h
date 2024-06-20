// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchActor.h"
#include "RoadSplineActor.generated.h"

struct FSplinePoint;
class USplineComponent;
/**
 * 
 */
UCLASS()
class ARCHVIZ_API ARoadSplineActor : public AArchActor
{
	GENERATED_BODY()



public:
	ARoadSplineActor();

	void OnConstruction(const FTransform& Transform) override;

	void DestroyRoad();

	void UpdateRoad();

	void UpdateRoad2();


	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USplineComponent* Spline;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* SplineMesh;

	UPROPERTY()
	TArray<FVector> SplinePoints;

	UPROPERTY()
	TArray<UStaticMeshComponent*> SplineMeshComponents;


};
