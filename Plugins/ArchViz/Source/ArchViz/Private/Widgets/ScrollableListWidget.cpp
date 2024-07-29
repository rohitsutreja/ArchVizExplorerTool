// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ScrollableListWidget.h"
#include "Widgets/MaterialItemWidget.h"
#include "Components/ScrollBox.h"


void UScrollableListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PopulateMaterialList();
}

void UScrollableListWidget::PopulateMaterialList()
{
	if (!MaterialDataAsset || !MaterialScrollBox)
	{
		return;
	}

	MaterialScrollBox->ClearChildren();

	MaterialScrollBox->SetOrientation(Orientation);

	TArray<FMaterialInfo> ArrayOfMaterials;

	switch (TypeOfMaterial)
	{
	case EArchItem::Road:
	{
		ArrayOfMaterials = MaterialDataAsset->ArrayOfRoadMaterials;
	}
	break;

	case EArchItem::Wall:
	{
		ArrayOfMaterials = MaterialDataAsset->ArrayOfWallMaterials;
	}
	break;

	case EArchItem::Floor: 
	{
		ArrayOfMaterials = MaterialDataAsset->ArrayOfFloorMaterials;
	}
	break;
	case EArchItem::Door:
	{
		ArrayOfMaterials = MaterialDataAsset->ArrayOfDoorMaterials;
	}
	break;

	case EArchItem::Ceiling:
	{
		ArrayOfMaterials = MaterialDataAsset->ArrayOFCeilingMaterials;
	}
		break;
	case EArchItem::DoorFrame:
	{
		ArrayOfMaterials = MaterialDataAsset->ArrayOFDoorFrameMaterials;
	}
		break;
	}

	for (const FMaterialInfo& MaterialInfo : ArrayOfMaterials)
	{
		if (UMaterialItemWidget* EntryWidget = CreateWidget<UMaterialItemWidget>(this, MaterialItemClass))
		{
			EntryWidget->SetMaterialInfo(MaterialInfo);
			EntryWidget->OnMaterialEntryClicked.AddUniqueDynamic(this, &UScrollableListWidget::HandleMaterialItemClicked);
			MaterialScrollBox->AddChild(EntryWidget);
		}
	}
}

void UScrollableListWidget::HandleMaterialItemClicked(FMaterialInfo MaterialInfo)
{
	OnMaterialChange.Broadcast(MaterialInfo);
}