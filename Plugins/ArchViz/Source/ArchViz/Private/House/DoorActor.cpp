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
    DoorComponent->SetRelativeLocation({0,45,0});
}



void ADoorActor::OpenDoor()
{

        FRotator NewRotation{0};
        NewRotation.Yaw =90;
        DoorComponent->SetRelativeRotation(NewRotation);

        bIsOpen = true;


}

void ADoorActor::CloseDoor()
{
        FRotator NewRotation{0};
        NewRotation.Yaw = 0;
        DoorComponent->SetRelativeRotation(NewRotation);

        bIsOpen = false;
    
}


void ADoorActor::ToggleDoor()
{
	if(bIsOpen)
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

void ADoorActor::SetDoorMaterial(FMaterialInfo MaterialInfo)
{
    DoorComponent->SetMaterial(0, MaterialInfo.Material);
}

void ADoorActor::SetDoorFrameMaterial(FMaterialInfo MaterialInfo)
{
    DoorFrameComponent->SetMaterial(0, MaterialInfo.Material);
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

        PropertyPanelUI->DoorOpenCloseButton->OnClicked.AddDynamic(this, &ADoorActor::ToggleDoor);

        PropertyPanelUI->DoorMaterialList->OnMaterialChange.AddDynamic(this, &ADoorActor::SetDoorMaterial);
        PropertyPanelUI->DoorFrameMaterialList->OnMaterialChange.AddDynamic(this, &ADoorActor::SetDoorFrameMaterial);




    }


    SetActorRotation({ 0,90,0 });
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
void ADoorActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (EndPlayReason == EEndPlayReason::Type::Destroyed)
    {
        DetachFromWall();
    }

	Super::EndPlay(EndPlayReason);
}

