// Fill out your copyright notice in the Description page of Project Settings.

#include "House/WallActor.h"
#include "ComponentUtils.h"
#include "Components/SpinBox.h"
#include "Components/TextBlock.h"
#include "House/DoorActor.h"
#include "Widgets/ScrollableListWidget.h"

AWallActor::AWallActor()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    NumberOfWallSegments = 5;
    LengthOfSegment = 200;
    WallSegmentMesh = nullptr;
    ArrayOfWallSegments.Empty();
}

void AWallActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
}

void AWallActor::BeginPlay()
{
    Super::BeginPlay();

    if (IsValid(PropertyPanelUI))
    {
        PropertyPanelUI->SwitchToWidget(0);
        PropertyPanelUI->Title->SetText(FText::FromString(TEXT("Wall")));
        PropertyPanelUI->WallLengthValue->OnValueChanged.AddDynamic(this, &AWallActor::OnLengthChange);
        PropertyPanelUI->WallMaterialList->OnMaterialChange.AddDynamic(this, &AWallActor::OnMaterialChange);
    }

    UpdateWall();
}

void AWallActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (EndPlayReason == EEndPlayReason::Destroyed)
    {
        RemoveAllDoorsFromWall();
    }

    Super::EndPlay(EndPlayReason);
}

void AWallActor::UpdateWall()
{
    for (auto& Temp : ArrayOfWallSegments)
    {
        if (IsValid(Temp))
        {
            Temp->DestroyComponent();
        }
    }
    ArrayOfWallSegments.Empty();

    for (int i = 0; i < NumberOfWallSegments; i++)
    {
        auto WallSegment = NewObject<UStaticMeshComponent>(this);
        WallSegment->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        WallSegment->SetStaticMesh(WallSegmentMesh);
        WallSegment->SetRelativeLocation(FVector(i * LengthOfSegment, 0, 0));
        WallSegment->RegisterComponent();
        if (IsValid(Material))
        {
            WallSegment->SetMaterial(0, Material);
        }
        ArrayOfWallSegments.Add(WallSegment);
    }

    for (auto& [idx, Door] : IndexToDoorMapping)
    {
        if (idx < ArrayOfWallSegments.Num())
        {
            if (IsValid(ArrayOfWallSegments[idx]))
            {
                ArrayOfWallSegments[idx]->SetStaticMesh(DoorHallSegment);
                Door->AttachToComponent(ArrayOfWallSegments[idx], FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("DoorSocket")));
                Door->SetActorRelativeLocation(FVector::ZeroVector);
                Door->SetActorRelativeRotation(FRotator::ZeroRotator);
            }
        }
    }


    for (auto& [idx, Window] : IndexToWindowMapping)
    {
        if (idx < ArrayOfWallSegments.Num())
        {
            if (IsValid(ArrayOfWallSegments[idx]))
            {
                ArrayOfWallSegments[idx]->SetStaticMesh(WindowSegmentMesh);
                Window->AttachToComponent(ArrayOfWallSegments[idx], FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("WindowSocket")));
                Window->SetActorRelativeLocation(FVector::ZeroVector);
                Window->SetActorRelativeRotation(FRotator::ZeroRotator);
            }
        }
    }

    HighLightBorder();
}

void AWallActor::OnLengthChange(float Length)
{
    auto NewNumberOfWallSegments = FMath::Floor(Length / LengthOfSegment);

    for (auto& [idx, Door] : IndexToDoorMapping)
    {
        if (idx > NewNumberOfWallSegments - 1)
        {
            ADoorActor* RemovedDoor = IndexToDoorMapping.FindAndRemoveChecked(idx);

            if (IsValid(RemovedDoor))
            {
                RemovedDoor->Destroy();
            }
        }
    }

    for (auto& [idx, Window] : IndexToWindowMapping)
    {
        if (idx > NewNumberOfWallSegments - 1)
        {
            AWindowActor* RemovedWindow = IndexToWindowMapping.FindAndRemoveChecked(idx);

            if (IsValid(RemovedWindow))
            {
                RemovedWindow->Destroy();
            }
        }
    }

    if(NumberOfWallSegments != NewNumberOfWallSegments)
    {
        NumberOfWallSegments = NewNumberOfWallSegments;
        UpdateWall();
    }

}

void AWallActor::HighLightBorder()
{
    for (auto& Temp : ArrayOfWallSegments)
    {
        if (IsValid(Temp))
        {
            Temp->SetRenderCustomDepth(true);
            Temp->CustomDepthStencilValue = 2;
        }
    }
}

void AWallActor::UnHighLightBorder()
{
    for (auto& Temp : ArrayOfWallSegments)
    {
        if (IsValid(Temp))
        {
            Temp->SetRenderCustomDepth(false);
        }
    }
}

void AWallActor::OnMaterialChange(FMaterialInfo MaterialInfo)
{
    SetMaterial(MaterialInfo.Material);
    UpdateWall();
}

bool AWallActor::AttachDoorToComponent(UStaticMeshComponent* Component, ADoorActor* Door)
{
    int32 WallSegmentIdx = ArrayOfWallSegments.Find(Component);

    if (WallSegmentIdx != INDEX_NONE)
    {
        if(!IndexToWindowMapping.Find(WallSegmentIdx))
        {
	        IndexToDoorMapping.Add({ WallSegmentIdx, Door });

	        UpdateWall();

	        if (WallSegmentIdx < ArrayOfWallSegments.Num())
	        {
	            Door->AttachToComponent(ArrayOfWallSegments[WallSegmentIdx], FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("DoorSocket")));
	            Door->SetActorRelativeLocation(FVector::ZeroVector);
	            Door->SetActorRelativeRotation(FRotator::ZeroRotator);
	            Door->ParentWallComponentIndex = WallSegmentIdx;

                return true;
	        }
        }
    }

    return false;
}

void AWallActor::DetachDoorFromComponent(UStaticMeshComponent* Component)
{
    int32 WallSegmentIdx = ArrayOfWallSegments.Find(Component);

    if (WallSegmentIdx != INDEX_NONE)
    {
        IndexToDoorMapping.Remove(WallSegmentIdx);

    }

    UpdateWall();
    UnHighLightBorder();
}

void AWallActor::RemoveAllDoorsFromWall()
{
    for (auto& [idx, Door] : IndexToDoorMapping)
    {
        if (IsValid(Door))
        {
            Door->Destroy();
        }
    }

    IndexToDoorMapping.Empty();
    UpdateWall();
}

void AWallActor::AddDoorAtIndex(int32 Idx, ADoorActor* Door)
{
    IndexToDoorMapping.Add({ Idx, Door });
}

bool AWallActor::AttachWindowToComponent(UStaticMeshComponent* Component, AWindowActor* Window)
{
    int32 WallSegmentIdx = ArrayOfWallSegments.Find(Component);

    if (WallSegmentIdx != INDEX_NONE)
    {
        if(!IndexToDoorMapping.Find(WallSegmentIdx))
        {
			 IndexToWindowMapping.Add({ WallSegmentIdx, Window });

             if (WallSegmentIdx < ArrayOfWallSegments.Num())
             {
                 Window->AttachToComponent(ArrayOfWallSegments[WallSegmentIdx], FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("WindowSocket")));
                 Window->SetActorRelativeLocation(FVector::ZeroVector);
                 Window->SetActorRelativeRotation(FRotator::ZeroRotator);
                 Window->ParentWallComponentIndex = WallSegmentIdx;
             }

             UpdateWall();

             return true;
             
        }
    }

    return false;
}

void AWallActor::DetachWindowFromComponent(UStaticMeshComponent* Component)
{
    int32 WallSegmentIdx = ArrayOfWallSegments.Find(Component);

    if (WallSegmentIdx != INDEX_NONE)
    {
        IndexToWindowMapping.Remove(WallSegmentIdx);

    }

    UpdateWall();

    UnHighLightBorder();
}

void AWallActor::RemoveAllWindowsFromWall()
{
    for (auto& [idx, Door] : IndexToWindowMapping)
    {
        if (IsValid(Door))
        {
            Door->Destroy();
        }
    }

    IndexToWindowMapping.Empty();
    UpdateWall();
}

void AWallActor::AddWindowAtIndex(int32 Idx, AWindowActor* Window)
{
    IndexToWindowMapping.Add({ Idx, Window });

}

void AWallActor::SetMaterial(UMaterialInterface* InMaterial)
{
    Material = InMaterial;
}

void AWallActor::SynchronizePropertyPanel()
{
    if(IsValid(PropertyPanelUI))
    {
        PropertyPanelUI->WallLengthValue->SetValue(NumberOfWallSegments * LengthOfSegment);
    }
}

// Getters
int32 AWallActor::GetNumberOfWallSegments() const
{
    return NumberOfWallSegments;
}

int32 AWallActor::GetLengthOfSegment() const
{
    return LengthOfSegment;
}

UStaticMesh* AWallActor::GetWallSegmentMesh() const
{
    return WallSegmentMesh;
}

UStaticMesh* AWallActor::GetDoorHallSegment() const
{
    return DoorHallSegment;
}

UMaterialInterface* AWallActor::GetMaterial() const
{
    return Material;
}

const TArray<UStaticMeshComponent*>& AWallActor::GetArrayOfWallSegments() const
{
    return ArrayOfWallSegments;
}

const TMap<int32, ADoorActor*>& AWallActor::GetIndexToDoorMapping() const
{
    return IndexToDoorMapping;
}

// Setters
void AWallActor::SetNumberOfWallSegments(int32 InNumberOfWallSegments)
{
    NumberOfWallSegments = InNumberOfWallSegments;
    UpdateWall();
}

void AWallActor::SetLengthOfSegment(int32 InLengthOfSegment)
{
    LengthOfSegment = InLengthOfSegment;
    UpdateWall();
}

void AWallActor::SetWallSegmentMesh(UStaticMesh* InWallSegmentMesh)
{
    WallSegmentMesh = InWallSegmentMesh;
    UpdateWall();
}

void AWallActor::SetDoorHallSegment(UStaticMesh* InDoorHallSegment)
{
    DoorHallSegment = InDoorHallSegment;
    UpdateWall();
}

void AWallActor::SetArrayOfWallSegments(const TArray<UStaticMeshComponent*>& InArrayOfWallSegments)
{
    ArrayOfWallSegments = InArrayOfWallSegments;
}

void AWallActor::SetIndexToDoorMapping(const TMap<int32, ADoorActor*>& InIndexToDoorMapping)
{
    IndexToDoorMapping = InIndexToDoorMapping;
}

void AWallActor::SetLength(float Length)
{
    LengthOfSegment = Length;
    UpdateWall();
}
