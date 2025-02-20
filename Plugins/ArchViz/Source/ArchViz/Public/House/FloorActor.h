// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "House/HouseComponent.h"
#include "ProceduralMeshComponent.h"
#include "DataAssets/MaterialDataAsset.h"
#include "SaveAndLoad/ArchVizSave.h"
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

    // Overrides
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void BeginPlay() override;

    // Methods
    void GenerateFloor();
    virtual void HighLightBorder() override;
    virtual void UnHighLightBorder() override;

    void SyncProperties() override;
    void SynchronizePropertyPanel();
    void AdjustOffset();


    // Getters
    const FVector& GetDimensions() const;
    UMaterialInterface* GetBottomMaterial() const;
    UMaterialInterface* GetTopMaterial() const;
    const FVector& GetOffset() const;


    // Setters
    void SetDimensions(const FVector& InDimensions);
    void SetOffset(const FVector& InOffset);
    void SetBottomMaterial(UMaterialInterface* InMaterial);
    void SetTopMaterial(UMaterialInterface* InMaterial);

    //UFUNCTIONs
    UFUNCTION()
    void OnFloorMaterialChange(FMaterialInfo MaterialInfo);

    UFUNCTION()
    void OnCeilingMaterialChange(FMaterialInfo MaterialInfo);

    UFUNCTION()
    void OnDimensionsChange(float X);

    FVector OldDimensions;
    FVector EndPoint{0};

private:
    UPROPERTY(EditAnywhere)
    FVector Dimensions;

 

    UPROPERTY()
    UProceduralMeshComponent* ProcMesh;

    UPROPERTY(EditAnywhere)
    UMaterialInterface* BottomMaterial;


    UPROPERTY(EditAnywhere)
    UMaterialInterface* TopMaterial;

    UPROPERTY()
    UMaterialInstanceDynamic* DynBottomMaterial;

    UPROPERTY()
    UMaterialInstanceDynamic* DynTopMaterial;
};
