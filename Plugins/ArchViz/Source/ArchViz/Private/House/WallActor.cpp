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
        PropertyPanelUI->WallLengthValue->OnValueChanged.AddUniqueDynamic(this, &AWallActor::OnLengthChange);
        PropertyPanelUI->WallMaterialList->OnMaterialChange.AddUniqueDynamic(this, &AWallActor::OnMaterialChange);
    }

    UpdateWall();
}

void AWallActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (EndPlayReason == EEndPlayReason::Destroyed)
    {
        DestroyAllDoorsFromWall();
        DestroyAllWindowsFromWall();
    }

    Super::EndPlay(EndPlayReason);
}

void AWallActor::UpdateWall()
{
    GenerateLargeWallSegments();

    GenerateSmallWallSegments();

    for (auto& [idx, Door] : IndexToDoorMapping)
    {
        if (idx < ArrayOfWallSegments.Num())
        {
            ArrayOfWallSegments[idx]->SetStaticMesh(DoorHallSegment);
            AttachActorToWallSegment(Door, ArrayOfWallSegments[idx], "DoorSocket");
        }
    }

    for (auto& [idx, Window] : IndexToWindowMapping)
    {
        if (idx < ArrayOfWallSegments.Num())
        {
            ArrayOfWallSegments[idx]->SetStaticMesh(WindowSegmentMesh);
            AttachActorToWallSegment(Window, ArrayOfWallSegments[idx], "WindowSocket");
        }
    }

    HighLightBorder();
}

void AWallActor::GenerateLargeWallSegments()
{
    int32 CurrentNumSegments = ArrayOfWallSegments.Num();

    if (CurrentNumSegments > NumberOfWallSegments)
    {
        for (int32 i = NumberOfWallSegments; i < CurrentNumSegments; i++)
        {
            if (ArrayOfWallSegments[i])
            {
                ArrayOfWallSegments[i]->DestroyComponent();
            }
        }
        ArrayOfWallSegments.SetNum(NumberOfWallSegments);
    }
    else if (CurrentNumSegments < NumberOfWallSegments)
    {
        for (int32 i = CurrentNumSegments; i < NumberOfWallSegments; i++)
        {
            auto WallSegment = NewObject<UStaticMeshComponent>(this);
            WallSegment->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
            WallSegment->RegisterComponent();
            ArrayOfWallSegments.Add(WallSegment);
        }
    }

    for (int32 i = 0; i < NumberOfWallSegments; i++)
    {
        ArrayOfWallSegments[i]->SetStaticMesh(WallSegmentMesh);
        ArrayOfWallSegments[i]->SetRelativeLocation(FVector(i * LengthOfSegment, 0, 0));
        if (IsValid(Material))
        {
            ArrayOfWallSegments[i]->SetMaterial(0, Material);
        }
    }
}
void AWallActor::GenerateSmallWallSegments()
{
    if (NumberOfSmallWallSegments <= 0)
    {
        return;
    }

    int32 CurrentSmallWallSegments = ArrayOfSmallWallSegments.Num();

    if (CurrentSmallWallSegments > NumberOfSmallWallSegments)
    {
        for (int32 i = NumberOfSmallWallSegments; i < CurrentSmallWallSegments; i++)
        {
            if (ArrayOfSmallWallSegments[i])
            {
                ArrayOfSmallWallSegments[i]->DestroyComponent();
            }
        }
        ArrayOfSmallWallSegments.SetNum(NumberOfSmallWallSegments);
    }
    else if (CurrentSmallWallSegments < NumberOfSmallWallSegments)
    {
        for (int32 i = CurrentSmallWallSegments; i < NumberOfSmallWallSegments; i++)
        {
            auto WallSegment = NewObject<UStaticMeshComponent>(this);
            WallSegment->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
            WallSegment->RegisterComponent();
            ArrayOfSmallWallSegments.Add(WallSegment);
        }
    }

    int32 Offset = (NumberOfWallSegments)*LengthOfSegment;

    for (int32 i = 0; i < NumberOfSmallWallSegments; i++)
    {
        ArrayOfSmallWallSegments[i]->SetStaticMesh(SmallWallSegment);
        ArrayOfSmallWallSegments[i]->SetRelativeLocation(FVector((Offset > 0 ? Offset : 0) + i * 20, 0, 0));
        if (IsValid(Material))
        {
            ArrayOfSmallWallSegments[i]->SetMaterial(0, Material);
        }
    }
}

void AWallActor::AttachActorToWallSegment(AActor* Actor, UStaticMeshComponent* WallSegment, const FName& SocketName)
{
    if (IsValid(WallSegment) && IsValid(Actor))
    {
        Actor->AttachToComponent(WallSegment, FAttachmentTransformRules::KeepRelativeTransform, SocketName);
        Actor->SetActorRelativeLocation(FVector::ZeroVector);
        Actor->SetActorRelativeRotation(FRotator::ZeroRotator);
    }
}

void AWallActor::OnLengthChange(float Length)
{
    int32 NewNumberOfWallSegments = FMath::Floor(Length / LengthOfSegment);

    int32 NewSmallSegments = (((static_cast<int32>(Length) / 20) * 20) % LengthOfSegment) / 20;

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
        NumberOfSmallWallSegments = NewSmallSegments;
        UpdateWall();
        return;
    }

    if (NumberOfSmallWallSegments != NewSmallSegments)
    {
        NumberOfSmallWallSegments = NewSmallSegments;
        UpdateWall();
    }

}

void AWallActor::HighLightBorder()
{
    for (auto& WallSegment : ArrayOfWallSegments)
    {
        if (IsValid(WallSegment))
        {
            WallSegment->SetRenderCustomDepth(true);
            WallSegment->CustomDepthStencilValue = 2;
        }
    }

    for (auto& WallSegment : ArrayOfSmallWallSegments)
    {
        if (IsValid(WallSegment))
        {
            WallSegment->SetRenderCustomDepth(true);
            WallSegment->CustomDepthStencilValue = 2;
        }
    }
}

void AWallActor::UnHighLightBorder()
{
    for (auto& WallSegment : ArrayOfWallSegments)
    {
        if (IsValid(WallSegment))
        {
            WallSegment->SetRenderCustomDepth(false);
        }
    }

    for (auto& WallSegment : ArrayOfSmallWallSegments)
    {
        if (IsValid(WallSegment))
        {
            WallSegment->SetRenderCustomDepth(false);
        }
    }
}

void AWallActor::OnMaterialChange(FMaterialInfo MaterialInfo)
{
    SetMaterial(MaterialInfo.Material);

    for (auto& WallSegment : ArrayOfWallSegments)
    {
        if (IsValid(WallSegment))
        {
            WallSegment->SetMaterial(0, Material);
        }
    }

    for (auto& WallSegment : ArrayOfSmallWallSegments)
    {
        if (IsValid(WallSegment))
        {
            WallSegment->SetMaterial(0, Material);
        }
    }
}

bool AWallActor::AttachDoorToComponent(UStaticMeshComponent* Component, ADoorActor* Door)
{
    int32 WallSegmentIdx = ArrayOfWallSegments.Find(Component);

    if (WallSegmentIdx != INDEX_NONE)
    {
        if(!IndexToWindowMapping.Contains(WallSegmentIdx) && !IndexToDoorMapping.Contains(WallSegmentIdx))
        {
	        if (WallSegmentIdx < ArrayOfWallSegments.Num())
	        {
                IndexToDoorMapping.Add({ WallSegmentIdx, Door });

                ArrayOfWallSegments[WallSegmentIdx]->SetStaticMesh(DoorHallSegment);

                AttachActorToWallSegment(Door, ArrayOfWallSegments[WallSegmentIdx], "DoorSocket");

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
        if (IndexToDoorMapping.Contains(WallSegmentIdx))
        {
            if (auto Door = IndexToDoorMapping.FindAndRemoveChecked(WallSegmentIdx); IsValid(Door))
            {
                Door->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
                Door->ParentWallComponentIndex = -1;
                ArrayOfWallSegments[WallSegmentIdx]->SetStaticMesh(WallSegmentMesh);
            }
        }
       
    }
}

void AWallActor::DestroyAllDoorsFromWall()
{
    for (auto& [Idx, Door] : IndexToDoorMapping)
    {
        if (IsValid(Door))
        {
            Door->Destroy();
        }
    }

    IndexToDoorMapping.Empty();
   
}

void AWallActor::AddDoorAtIndex(int32 Idx, ADoorActor* Door)
{
    IndexToDoorMapping.Add({ Idx, Door });
}

void AWallActor::AttachWindowAtIndex(int32 Idx, AWindowActor* Window)
{
    IndexToWindowMapping.Add({ Idx, Window });
}

bool AWallActor::AttachWindowToComponent(UStaticMeshComponent* Component, AWindowActor* Window)
{
    int32 WallSegmentIdx = ArrayOfWallSegments.Find(Component);

    if (WallSegmentIdx != INDEX_NONE)
    {
        if(!IndexToDoorMapping.Find(WallSegmentIdx) && !IndexToWindowMapping.Find(WallSegmentIdx))
        {
             if (WallSegmentIdx < ArrayOfWallSegments.Num())
             {
                 IndexToWindowMapping.Add({ WallSegmentIdx, Window });
                 ArrayOfWallSegments[WallSegmentIdx]->SetStaticMesh(WindowSegmentMesh);
                 AttachActorToWallSegment(Window, ArrayOfWallSegments[WallSegmentIdx], "WindowSocket");
                 Window->ParentWallComponentIndex = WallSegmentIdx;
             }
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
        if(IndexToWindowMapping.Contains(WallSegmentIdx))
        {
	        if (auto Window = IndexToWindowMapping.FindAndRemoveChecked(WallSegmentIdx); IsValid(Window))
            {
                Window->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
                Window->ParentWallComponentIndex = -1;
                ArrayOfWallSegments[WallSegmentIdx]->SetStaticMesh(WallSegmentMesh);
            }
        }

    }
}

void AWallActor::DestroyAllWindowsFromWall()
{
    for (auto& [Idx, Window] : IndexToWindowMapping)
    {
        if (IsValid(Window))
        {
            Window->Destroy();

        }
    }

    IndexToWindowMapping.Empty();
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
        PropertyPanelUI->WallLengthValue->SetValue(NumberOfWallSegments * LengthOfSegment + NumberOfSmallWallSegments * 20);
    }
}

void AWallActor::SyncProperties()
{
    SynchronizePropertyPanel();
}

// Getters
int32 AWallActor::GetNumberOfWallSegments() const
{
    return NumberOfWallSegments;
}

int32 AWallActor::GetNumberOfSmallWallSegments() const
{
    return NumberOfSmallWallSegments;
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

void AWallActor::SetNumberOfSmallWallSegments(int32 InNumberOfSmallWallSegments)
{
    NumberOfSmallWallSegments = InNumberOfSmallWallSegments;
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
    int32 NewNumberOfLargeWallSegments = FMath::Floor(Length / LengthOfSegment);

    int32 NewNumberOfSmallSegments = (((static_cast<int32>(Length) / 20) * 20) % LengthOfSegment) / 20;

    if(NewNumberOfLargeWallSegments == 0)
    {
        NewNumberOfSmallSegments = FMath::Max(NewNumberOfSmallSegments, 1);
    }

	if(NewNumberOfLargeWallSegments != NumberOfWallSegments)
    {
        NumberOfWallSegments = NewNumberOfLargeWallSegments;
        NumberOfSmallWallSegments = NewNumberOfSmallSegments;
        UpdateWall();
        return;
    }

    if (NewNumberOfSmallSegments != NumberOfSmallWallSegments)
    {
        NumberOfSmallWallSegments = NewNumberOfSmallSegments;
        UpdateWall();
    }
}
