// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MaterialItemWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UMaterialItemWidget::SetMaterialInfo(const FMaterialInfo& Info)
{
    MaterialInfo = Info;

    if(MaterialInfo.Thumbnail)
    {
        MaterialThumbnail->SetBrushResourceObject(MaterialInfo.Thumbnail);
    }
}

void UMaterialItemWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (MaterialThumbnail)
    {
       MaterialButton->OnClicked.AddDynamic(this, &UMaterialItemWidget::HandleMaterialButtonClicked);
    }
}

void UMaterialItemWidget::HandleMaterialButtonClicked()
{
    OnMaterialEntryClicked.Broadcast(MaterialInfo);
}
