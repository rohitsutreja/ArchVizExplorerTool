#pragma once

#include "CoreMinimal.h"
#include "ArchActor.h"
#include "DataAssets/MaterialDataAsset.h"
#include "RoadSplineActor.generated.h"


DECLARE_DELEGATE(FOnRoadDelete);

UENUM()
enum class ERoadType
{
    Sharp,
    Curve
};

class USplineMeshComponent;
struct FSplinePoint;
class USplineComponent;



UCLASS()
class ARCHVIZ_API ARoadSplineActor : public AArchActor
{
    GENERATED_BODY()

public:
    ARoadSplineActor();

    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


    void DestroyRoad();
    void AddSplinePoint(const FVector& Location);
    bool RemoveLastSplinePoint();
    void UpdateRoad();

    void SynchronizePropertyPanel();

    virtual void HighLightBorder() override;
    virtual void UnHighLightBorder() override;


    UStaticMesh* GetSourceStaticMesh() const;
    const TArray<FVector>& GetSplinePoints() const;
    UMaterialInterface* GetMaterial() const;
    float GetWidth() const;
    ERoadType GetTypeOfRoad() const;

    void SetSourceSplineMesh(UStaticMesh* InSplineMesh);
    void SetSplinePoints(const TArray<FVector>& InSplinePoints);
    void SetMaterial(UMaterialInterface* InMaterial);
    void SetWidth(float InWidth);
    void SetTypeOfRoad(ERoadType InTypeOfRoad);

    // Delegates
    FOnRoadDelete OnRoadDelete;

    // UI Property Bindings
    UFUNCTION()
    void OnWidthChanged(float InValue);

    UFUNCTION()
    void OnRoadTypeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnMaterialChange(FMaterialInfo MaterialInfo);

protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USplineComponent* Spline;

    UPROPERTY(EditDefaultsOnly)
    UStaticMesh* SourceStaticMesh;

    UPROPERTY()
    TArray<FVector> SplinePoints;

    UPROPERTY()
    TArray<USplineMeshComponent*> SplineMeshComponents;

    UPROPERTY(EditDefaultsOnly)
    UMaterialInterface* Material;

    UPROPERTY()
    float Width = 400.0f;

    UPROPERTY(EditDefaultsOnly)
    ERoadType TypeOfRoad = ERoadType::Sharp;
};
