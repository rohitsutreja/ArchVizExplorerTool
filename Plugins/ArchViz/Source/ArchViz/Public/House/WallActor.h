// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WindowActor.h"
#include "House/HouseComponent.h"
#include "SaveAndLoad/ArchVizSave.h"
#include "DataAssets/MaterialDataAsset.h"
#include "WallActor.generated.h"

class ADoorActor;



UCLASS()
class ARCHVIZ_API AWallActor : public AHouseComponent
{
    GENERATED_BODY()

public:
    AWallActor();

    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    void UpdateWall();
    void GenerateLargeWallSegments();
    void GenerateSmallWallSegments();
    void AttachActorToWallSegment(AActor* Actor, UStaticMeshComponent* WallSegment, const FName& SocketName);

    bool AttachDoorToComponent(UStaticMeshComponent* Component, ADoorActor* Door);
    void DetachDoorFromComponent(UStaticMeshComponent* Component);
    void DestroyAllDoorsFromWall();

    void AddDoorAtIndex(int32 Idx, ADoorActor* Door);
    void AttachWindowAtIndex(int32 Idx, AWindowActor* Window);



    bool AttachWindowToComponent(UStaticMeshComponent* Component, AWindowActor* Door);
    void DetachWindowFromComponent(UStaticMeshComponent* Component);
    void DestroyAllWindowsFromWall();
    void AddWindowAtIndex(int32 Idx, AWindowActor* Door);

    void SetMaterial(UMaterialInterface* InMaterial);

    void SynchronizePropertyPanel();
    virtual void SyncProperties() override;

    // Getters
    int32 GetNumberOfWallSegments() const;
    int32 GetNumberOfSmallWallSegments() const;
    int32 GetLengthOfSegment() const;
    UStaticMesh* GetWallSegmentMesh() const;
    UStaticMesh* GetDoorHallSegment() const;
    UMaterialInterface* GetMaterial() const;
    const TArray<UStaticMeshComponent*>& GetArrayOfWallSegments() const;
    const TMap<int32, ADoorActor*>& GetIndexToDoorMapping() const;

    // Setters
    void SetNumberOfWallSegments(int32 InNumberOfWallSegments);
    void SetNumberOfSmallWallSegments(int32 InNumberOfSmallWallSegments);
    void SetLengthOfSegment(int32 InLengthOfSegment);
    void SetWallSegmentMesh(UStaticMesh* InWallSegmentMesh);
    void SetDoorHallSegment(UStaticMesh* InDoorHallSegment);
    void SetArrayOfWallSegments(const TArray<UStaticMeshComponent*>& InArrayOfWallSegments);
    void SetIndexToDoorMapping(const TMap<int32, ADoorActor*>& InIndexToDoorMapping);
    void SetLength(float Length);

    // UFUNCTIONs
    UFUNCTION()
    void OnLengthChange(float Length);

    UFUNCTION()
    void OnMaterialChange(FMaterialInfo MaterialInfo);

    // Override Functions
    virtual void HighLightBorder() override;
    virtual void UnHighLightBorder() override;

protected:
    // Properties
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 NumberOfWallSegments;

    int32 NumberOfSmallWallSegments{0};

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 LengthOfSegment;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UStaticMesh* WallSegmentMesh;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UStaticMesh* DoorHallSegment;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UStaticMesh* WindowSegmentMesh;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UStaticMesh* SmallWallSegment;



    UPROPERTY()
    UMaterialInterface* Material;

    UPROPERTY()
    TArray<UStaticMeshComponent*> ArrayOfWallSegments;

    UPROPERTY()
    TArray<UStaticMeshComponent*> ArrayOfSmallWallSegments;

    UPROPERTY()
    TMap<int32, ADoorActor*> IndexToDoorMapping;

    UPROPERTY()
    TMap<int32, AWindowActor*> IndexToWindowMapping;
};
