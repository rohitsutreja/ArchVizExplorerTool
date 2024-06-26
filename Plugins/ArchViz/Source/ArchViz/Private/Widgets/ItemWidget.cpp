// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ItemWidget.h"
#include "Engine/Texture2D.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Engine/StaticMesh.h"
#include "Engine/Texture2D.h"
#include "ObjectTools.h"





void UItemWidget::SetItemInfo(const FItemInfo& Info)
{
    ItemInfo = Info;

    if (ItemInfo.Thumbnail)
    {
        ItemThumbnail->SetBrushResourceObject((Cast<UTexture2D>(Info.Thumbnail)));
    }
}




void UItemWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ItemThumbnail)
    {
        ItemButton->OnClicked.AddDynamic(this, &UItemWidget::HandleMaterialButtonClicked);
    }
}

void UItemWidget::HandleMaterialButtonClicked()
{
    OnItemEntryClicked.Broadcast(ItemInfo);
}
