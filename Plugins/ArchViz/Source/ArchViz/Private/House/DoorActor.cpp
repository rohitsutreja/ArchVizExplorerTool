// Fill out your copyright notice in the Description page of Project Settings.


#include "House/DoorActor.h"

#include "ComponentUtils.h"
#include "Components/SpinBox.h"
#include "Components/TextBlock.h"
#include "House/WallActor.h"

ADoorActor::ADoorActor()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create and set the root component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    SetRootComponent(RootComponent);

    // Create the static mesh component for the door
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
    StaticMeshComponent->SetupAttachment(RootComponent);
}

void ADoorActor::BeginPlay()
{
	Super::BeginPlay();

    StaticMeshComponent->SetStaticMesh(DoorMesh);

    if (IsValid(PropertyPanelUI))
    {
        PropertyPanelUI->SwitchToWidget(2);

        PropertyPanelUI->Title->SetText(FText::FromString(TEXT("Door")));
    }


    SetActorRotation({ 0,90,0 });
}

void ADoorActor::HighLightBorder()
{
    StaticMeshComponent->SetRenderCustomDepth(true);
    StaticMeshComponent->CustomDepthStencilValue = 2;
}

void ADoorActor::UnHighLightBorder()
{
    StaticMeshComponent->SetRenderCustomDepth(false);
}

void ADoorActor::Destroyed()
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

    Super::Destroyed();
}

