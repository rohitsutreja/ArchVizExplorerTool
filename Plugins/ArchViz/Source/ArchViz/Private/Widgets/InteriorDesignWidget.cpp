// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InteriorDesignWidget.h"

#include "Components/Border.h"
#include "Components/Button.h"

void UInteriorDesignWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BedButton->OnClicked.AddUniqueDynamic(this, &UInteriorDesignWidget::OnBedButtonClicked);
	ChairButton->OnClicked.AddUniqueDynamic(this, &UInteriorDesignWidget::OnChairButtonClicked);
	LightButton->OnClicked.AddUniqueDynamic(this, &UInteriorDesignWidget::OnLightButtonClicked);
	TableButton->OnClicked.AddUniqueDynamic(this, &UInteriorDesignWidget::OnTableButtonClicked);
	PictureButton->OnClicked.AddUniqueDynamic(this, &UInteriorDesignWidget::OnPictureButtonClicked);
	StairButton->OnClicked.AddUniqueDynamic(this, &UInteriorDesignWidget::OnStairButtonClicked);
	BathroomButton->OnClicked.AddUniqueDynamic(this, &UInteriorDesignWidget::OnBathroomButtonClicked);
	OtherItemsButton->OnClicked.AddUniqueDynamic(this, &UInteriorDesignWidget::OnOtherItemsButtonClicked);

}

void UInteriorDesignWidget::OnBedButtonClicked()
{
	MeshItemListBorder->SetVisibility(ESlateVisibility::Visible);
	MeshItemsScrollList->ItemDataAsset = BedMeshList;
	MeshItemsScrollList->PopulateItemsList();
}

void UInteriorDesignWidget::OnChairButtonClicked()
{
	MeshItemListBorder->SetVisibility(ESlateVisibility::Visible);

	MeshItemsScrollList->ItemDataAsset = ChairMeshList;
	MeshItemsScrollList->PopulateItemsList();
}

void UInteriorDesignWidget::OnTableButtonClicked()
{
	MeshItemListBorder->SetVisibility(ESlateVisibility::Visible);

	MeshItemsScrollList->ItemDataAsset = TableMeshList;
	MeshItemsScrollList->PopulateItemsList();
}

void UInteriorDesignWidget::OnPictureButtonClicked()
{
	MeshItemListBorder->SetVisibility(ESlateVisibility::Visible);

	MeshItemsScrollList->ItemDataAsset = PictureMeshList;
	MeshItemsScrollList->PopulateItemsList();
}

void UInteriorDesignWidget::OnLightButtonClicked()
{
	MeshItemListBorder->SetVisibility(ESlateVisibility::Visible);

	MeshItemsScrollList->ItemDataAsset = LightMeshList;
	MeshItemsScrollList->PopulateItemsList();
}

void UInteriorDesignWidget::OnCeilingLightButtonClicked()
{
	MeshItemListBorder->SetVisibility(ESlateVisibility::Visible);

	MeshItemsScrollList->ItemDataAsset = CeilingLightMeshList;
	MeshItemsScrollList->PopulateItemsList();
}


void UInteriorDesignWidget::OnStairButtonClicked()
{
	MeshItemListBorder->SetVisibility(ESlateVisibility::Visible);

	MeshItemsScrollList->ItemDataAsset = StairMeshList;
	MeshItemsScrollList->PopulateItemsList();
}

void UInteriorDesignWidget::OnBathroomButtonClicked()
{
	MeshItemListBorder->SetVisibility(ESlateVisibility::Visible);

	MeshItemsScrollList->ItemDataAsset = BathroomItemsList;
	MeshItemsScrollList->PopulateItemsList();
}

void UInteriorDesignWidget::OnOtherItemsButtonClicked()
{
	MeshItemListBorder->SetVisibility(ESlateVisibility::Visible);

	MeshItemsScrollList->ItemDataAsset = OtherItemsList;
	MeshItemsScrollList->PopulateItemsList();
}
