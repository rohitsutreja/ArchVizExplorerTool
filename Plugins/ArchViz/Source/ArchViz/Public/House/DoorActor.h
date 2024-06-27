// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "House/HouseComponent.h"
#include "DataAssets/MaterialDataAsset.h"
#include "DoorActor.generated.h"

/**
 *
 */

class AWallActor;

UCLASS()
class ARCHVIZ_API ADoorActor : public AHouseComponent
{
    GENERATED_BODY()

public:

    int32 ParentWallComponentIndex = -1;


    ADoorActor();

    // Overrides
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // Methods
    void OpenDoor();
    void CloseDoor();
    void DetachFromWall();

    void SynchronizePropertyPanel();


    //Getters
    UStaticMesh* GetDoorMesh() const;
    UStaticMesh* GetDoorFrameMesh() const;
    UMaterialInterface* GetDoorMaterial() const;
    UMaterialInterface* GetDoorFrameMaterial() const;


    bool IsDoorOpen() const;

    //Setters
    void SetDoorMesh(UStaticMesh* InDoorMesh);
    void SetDoorFrameMesh(UStaticMesh* InDoorFrameMesh);
    void SetDoorMaterial(UMaterialInterface* InMaterial);
    void SetDoorFrameMaterial(UMaterialInterface* InMaterial);

    //overrides
    virtual void HighLightBorder() override;
    virtual void UnHighLightBorder() override;


    //UFUNCTIONs
    UFUNCTION()
    void OnToggleDoor();

    UFUNCTION()
    void OnDoorMaterialChange(FMaterialInfo MaterialInfo);

    UFUNCTION()
    void OnDoorFrameMaterialChange(FMaterialInfo MaterialInfo);



private:
    UPROPERTY()
    UStaticMeshComponent* DoorFrameComponent;

    UPROPERTY()
    UStaticMeshComponent* DoorComponent;

    UPROPERTY(EditAnywhere)
    UStaticMesh* DoorMesh;

    UPROPERTY(EditAnywhere)
    UStaticMesh* DoorFrameMesh;

    bool bIsOpen;
};

