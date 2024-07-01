// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemsList.h"
#include "Blueprint/UserWidget.h"
#include "InteriorDesignWidget.generated.h"

class UBorder;
class UButton;
/**
 * 
 */
UCLASS()
class ARCHVIZ_API UInteriorDesignWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(meta = (BindWidget))
	UButton* BedButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ChairButton;

	UPROPERTY(meta = (BindWidget))
	UButton* PictureButton;

	UPROPERTY(meta = (BindWidget))
	UButton* TableButton;

	UPROPERTY(meta = (BindWidget))
	UButton* LightButton;

	UPROPERTY(meta = (BindWidget))
	UButton* StairButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BathroomButton;

	UPROPERTY(meta = (BindWidget))
	UButton* OtherItemsButton;


	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnBedButtonClicked();
	UFUNCTION()
	void OnChairButtonClicked();
	UFUNCTION()
	void OnTableButtonClicked();
	UFUNCTION()
	void OnPictureButtonClicked();
	UFUNCTION()
	void OnLightButtonClicked();
	UFUNCTION()
	void OnCeilingLightButtonClicked();
	UFUNCTION()
	void OnStairButtonClicked();
	UFUNCTION()
	void OnBathroomButtonClicked();
	UFUNCTION()
	void OnOtherItemsButtonClicked();


	UPROPERTY(meta = (BindWidget))
	UItemsList* MeshItemsScrollList;

	UPROPERTY(meta = (BindWidget))
	UBorder* MeshItemListBorder;

	UPROPERTY(EditAnywhere)
	UItemDataAsset* BedMeshList;

	UPROPERTY(EditAnywhere)
	UItemDataAsset* ChairMeshList;

	UPROPERTY(EditAnywhere)
	UItemDataAsset* LightMeshList;

	UPROPERTY(EditAnywhere)
	UItemDataAsset* CeilingLightMeshList;

	UPROPERTY(EditAnywhere)
	UItemDataAsset* TableMeshList;

	UPROPERTY(EditAnywhere)
	UItemDataAsset* PictureMeshList;

	UPROPERTY(EditAnywhere)
	UItemDataAsset* StairMeshList;


	UPROPERTY(EditAnywhere)
	UItemDataAsset* BathroomItemsList;


	UPROPERTY(EditAnywhere)
	UItemDataAsset* OtherItemsList;
};
