// Fill out your copyright notice in the Description page of Project Settings.


#include "House/WindowActor.h"

#include "ComponentUtils.h"
#include "Components/TextBlock.h"
#include "House/WallActor.h"
#include "Widgets/ScrollableListWidget.h"

AWindowActor::AWindowActor()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    SetRootComponent(RootComponent);

    WindowComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
    WindowComponent->SetupAttachment(RootComponent);
}

void AWindowActor::OnMaterialChange(FMaterialInfo MaterialInfo)
{
    Material = MaterialInfo.Material;

    if(IsValid(Material))
    {
        WindowComponent->SetMaterial(0, Material);
    }
}

void AWindowActor::BeginPlay()
{
	Super::BeginPlay();

    WindowComponent->SetStaticMesh(WindowMesh);


    if (IsValid(PropertyPanelUI))
    {
        PropertyPanelUI->SwitchToWidget(5);
        PropertyPanelUI->Title->SetText(FText::FromString(TEXT("Window")));
        PropertyPanelUI->WindowMaterialList->OnMaterialChange.AddDynamic(this, &AWindowActor::OnMaterialChange);

    }

}

void AWindowActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

    if (EndPlayReason == EEndPlayReason::Destroyed)
    {
        DetachFromWall();
    }
	Super::EndPlay(EndPlayReason);
}

void AWindowActor::HighLightBorder()
{
    WindowComponent->SetRenderCustomDepth(true);
    WindowComponent->CustomDepthStencilValue = 2;
}


void AWindowActor::UnHighLightBorder()
{
    WindowComponent->SetRenderCustomDepth(false);

}

void AWindowActor::DetachFromWall()
{
    if (auto ParentComp = ComponentUtils::GetAttachedParent(RootComponent))
    {
        if (auto ParentActor = ParentComp->GetAttachParentActor())
        {
            if (auto Wall = Cast<AWallActor>(ParentActor))
            {
                Wall->DetachWindowFromComponent(Cast<UStaticMeshComponent>(ParentComp));
            }
        }
    }
}

UMaterialInterface* AWindowActor::GetMaterial()
{
    return Material;
}
