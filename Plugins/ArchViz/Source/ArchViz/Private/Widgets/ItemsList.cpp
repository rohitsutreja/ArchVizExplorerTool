// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ItemsList.h"

#include "Components/ScrollBox.h"
#include "Widgets/ItemWidget.h"
#include "Widgets/MaterialItemWidget.h"

void UItemsList::PopulateItemsList()
{
	if (!ItemDataAsset || !ItemScrollBox)
	{
		return;
	}

	ItemScrollBox->ClearChildren();

	ItemScrollBox->SetOrientation(Orientation);


	for (const FItemInfo& ItemInfo : ItemDataAsset->ArrayOfItems)
	{
		if (UItemWidget* EntryWidget = CreateWidget<UItemWidget>(this, ItemWidgetClass))
		{
			EntryWidget->SetItemInfo(ItemInfo);
			EntryWidget->OnItemEntryClicked.AddDynamic(this, &UItemsList::HandleItemClicked);
			ItemScrollBox->AddChild(EntryWidget);
		}
	}
}

void UItemsList::NativeConstruct()
{
	Super::NativeConstruct();

	PopulateItemsList();

}

void UItemsList::HandleItemClicked(FItemInfo ItemInfo)
{
	OnItemChange.Broadcast(ItemInfo);

}
