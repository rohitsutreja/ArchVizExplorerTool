// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InteriorDesignWidget.h"

#include "Components/Border.h"
#include "Components/Button.h"

void UInteriorDesignWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BedButton->OnClicked.AddDynamic(this, &UInteriorDesignWidget::OnBedButtonClicked);
	ChairButton->OnClicked.AddDynamic(this, &UInteriorDesignWidget::OnChairButtonClicked);
	LightButton->OnClicked.AddDynamic(this, &UInteriorDesignWidget::OnLightButtonClicked);
	TableButton->OnClicked.AddDynamic(this, &UInteriorDesignWidget::OnTableButtonClicked);
	PictureButton->OnClicked.AddDynamic(this, &UInteriorDesignWidget::OnPictureButtonClicked);
	StairButton->OnClicked.AddDynamic(this, &UInteriorDesignWidget::OnStairButtonClicked);
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
