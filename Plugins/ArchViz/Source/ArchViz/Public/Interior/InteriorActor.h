// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchActor.h"
#include "InteriorActor.generated.h"

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
    WallPlaceable,
    FloorPlaceable,
    CeilingPlaceable,
    Table,
    TablePlaceable
};

UCLASS()
class ARCHVIZ_API AInteriorActor : public AArchActor
{
    GENERATED_BODY()

public:
    AInteriorActor();

    // Getters
    UStaticMeshComponent* GetStaticMeshComponent() const;
    UStaticMesh* GetCurrentStaticMesh() const;
    EItemCategory GetCategory() const;

    // Setters
    void SetStaticMesh(UStaticMesh* InStaticMesh);
    void SetCategory(EItemCategory InCategory);

    virtual void HighLightBorder() override;
    virtual void UnHighLightBorder() override;
    virtual void BeginPlay() override;

    void SnapActorToGrid(const FVector& GridSize);
    void Rotate();

protected:
    UPROPERTY()
    UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY()
    UStaticMesh* CurrentStaticMesh;

    UPROPERTY()
    EItemCategory Category;
};
