// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchActor.h"
#include "Widgets/PropertyPanelWidget.h"
#include "RoadSplineActor.generated.h"

class USplineMeshComponent;
struct FSplinePoint;
class USplineComponent;
/**
 *
 */



DECLARE_DELEGATE(FOnRoadDelete);

UENUM()
enum class ERoadType
{
	Sharp,
	Curve
};

UCLASS()
class ARCHVIZ_API ARoadSplineActor : public AArchActor
{
	GENERATED_BODY()



public:

	FOnRoadDelete OnRoadDelete;

	ARoadSplineActor();

	void OnConstruction(const FTransform& Transform) override;

	void DestroyRoad();

	void AddSplinePoint(const FVector& Location);

	void UpdateRoad();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USplineComponent* Spline;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* SplineMesh;

	UPROPERTY()
	TArray<FVector> SplinePoints;

	UPROPERTY()
	TArray<USplineMeshComponent*> SplineMeshComponents;

	UPROPERTY()
	UMaterialInterface* Material;

	UPROPERTY()
	float Width = 400;

	UPROPERTY(EditDefaultsOnly)
	ERoadType TypeOfRoad = ERoadType::Sharp;


	UFUNCTION()
	void OnWidthChanged(float InValue);

	UFUNCTION()
	void OnRoadTypeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);


	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	

	virtual void HighLightBorder() override;
	virtual void UnHighLightBorder() override;


	UFUNCTION()
	void SetMaterial(FMaterialInfo MaterialInfo);
};
