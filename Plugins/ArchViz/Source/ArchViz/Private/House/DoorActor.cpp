// Fill out your copyright notice in the Description page of Project Settings.

#include "House/DoorActor.h"
#include "House/WallActor.h"
#include "ComponentUtils.h"
#include "Widgets/ScrollableListWidget.h"

ADoorActor::ADoorActor()
{
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    SetRootComponent(RootComponent);

    DoorFrameComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
    DoorFrameComponent->SetupAttachment(RootComponent);

    DoorComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
    DoorComponent->SetupAttachment(DoorFrameComponent);
    DoorComponent->SetRelativeLocation(FVector(0, 45, 0)); 

    bIsOpen = false;
}

void ADoorActor::BeginPlay()
{
    Super::BeginPlay();

    DoorFrameComponent->SetStaticMesh(DoorFrameMesh);

    DoorComponent->SetStaticMesh(DoorMesh);

    if (IsValid(PropertyPanelUI))
    {
        PropertyPanelUI->SwitchToWidget(2);
        PropertyPanelUI->Title->SetText(FText::FromString(TEXT("Door")));

        PropertyPanelUI->DoorOpenCloseButton->OnClicked.AddUniqueDynamic(this, &ADoorActor::OnToggleDoor);

        PropertyPanelUI->DoorMaterialList->OnMaterialChange.AddUniqueDynamic(this, &ADoorActor::OnDoorMaterialChange);
        PropertyPanelUI->DoorFrameMaterialList->OnMaterialChange.AddUniqueDynamic(this, &ADoorActor::OnDoorFrameMaterialChange);
    }

    SetActorRotation(FRotator(0, 90, 0)); 
}

void ADoorActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (EndPlayReason == EEndPlayReason::Destroyed)
    {
        DetachFromWall();
    }

    Super::EndPlay(EndPlayReason);
}

void ADoorActor::OpenDoor()
{
    FRotator NewRotation(0, 90, 0); 
    DoorComponent->SetRelativeRotation(NewRotation);

    bIsOpen = true;
}

void ADoorActor::CloseDoor()
{
    FRotator NewRotation(0, 0, 0); 
    DoorComponent->SetRelativeRotation(NewRotation);

    bIsOpen = false;
}

void ADoorActor::SynchronizePropertyPanel()
{
    if (IsValid(PropertyPanelUI))
    {
        PropertyPanelUI->DoorOpenCloseText->SetText(FText::FromString((bIsOpen ? TEXT("Close") : TEXT("Open"))));
    }
}

void ADoorActor::OnToggleDoor()
{
    if (bIsOpen)
    {
        CloseDoor();
        PropertyPanelUI->DoorOpenCloseText->SetText(FText::FromString(TEXT("Open")));
    }
    else
    {
        OpenDoor();
        PropertyPanelUI->DoorOpenCloseText->SetText(FText::FromString(TEXT("Close")));
    }
}

void ADoorActor::OnDoorMaterialChange(FMaterialInfo MaterialInfo)
{
    SetDoorMaterial(MaterialInfo.Material);
}

void ADoorActor::OnDoorFrameMaterialChange(FMaterialInfo MaterialInfo)
{
    SetDoorFrameMaterial(MaterialInfo.Material);
}

void ADoorActor::HighLightBorder()
{
    DoorFrameComponent->SetRenderCustomDepth(true);
    DoorFrameComponent->CustomDepthStencilValue = 2;

    DoorComponent->SetRenderCustomDepth(true);
    DoorComponent->CustomDepthStencilValue = 2;
}

void ADoorActor::UnHighLightBorder()
{
    DoorFrameComponent->SetRenderCustomDepth(false);
    DoorComponent->SetRenderCustomDepth(false);
}

void ADoorActor::DetachFromWall()
{
    if (auto ParentComp = ComponentUtils::GetAttachedParent(RootComponent))
    {
        if (auto ParentActor = ParentComp->GetAttachParentActor())
        {
            if (auto Wall = Cast<AWallActor>(ParentActor))
            {
                Wall->DetachDoorFromComponent(Cast<UStaticMeshComponent>(ParentComp));
            }
        }
    }
}

void ADoorActor::SetDoorMaterial(UMaterialInterface* InMaterial)
{
    DoorComponent->SetMaterial(0, InMaterial);
}

void ADoorActor::SetDoorFrameMaterial(UMaterialInterface* InMaterial)
{
    DoorFrameComponent->SetMaterial(0, InMaterial);
}

// Getters
UStaticMesh* ADoorActor::GetDoorMesh() const
{
    return DoorMesh;
}

UStaticMesh* ADoorActor::GetDoorFrameMesh() const
{
    return DoorFrameMesh;
}

UMaterialInterface* ADoorActor::GetDoorMaterial() const
{
    return DoorComponent->GetMaterial(0);
}

UMaterialInterface* ADoorActor::GetDoorFrameMaterial() const
{
    return DoorFrameComponent->GetMaterial(0);

}

bool ADoorActor::IsDoorOpen() const
{
    return bIsOpen;
}

// Setters
void ADoorActor::SetDoorMesh(UStaticMesh* InDoorMesh)
{
    DoorMesh = InDoorMesh;
    DoorComponent->SetStaticMesh(DoorMesh);
}

void ADoorActor::SetDoorFrameMesh(UStaticMesh* InDoorFrameMesh)
{
    DoorFrameMesh = InDoorFrameMesh;
    DoorFrameComponent->SetStaticMesh(DoorFrameMesh);
}
