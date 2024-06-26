// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PropertyPanelWidget.generated.h"

class UScrollableListWidget;
class UComboBoxString;
class UTextBlock;
class USpinBox;
class UVerticalBox;
class UGridPanel;
class UWidgetSwitcher;
class UButton;


UCLASS()
class ARCHVIZ_API UPropertyPanelWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Title;
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	UGridPanel* WallGridPanel;

	UPROPERTY(meta = (BindWidget))
	UGridPanel* DoorGridPanel;

	UPROPERTY(meta = (BindWidget))
	UGridPanel* FloorGridPanel;



	UPROPERTY(meta = (BindWidget))
	UVerticalBox* WidgetSwitcherContainer;

	UPROPERTY(meta = (BindWidget))
	USpinBox* WallLengthValue;


	UPROPERTY(meta = (BindWidget))
	USpinBox* FloorLength;

	UPROPERTY(meta = (BindWidget))
	USpinBox* FloorWidth;

	UPROPERTY(meta = (BindWidget))
	USpinBox* FloorHeight;

	UPROPERTY(meta = (BindWidget))
	UButton* DeleteWallButton;

	UPROPERTY(meta = (BindWidget))
	UButton* DoorOpenCloseButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DoorOpenCloseText;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* RoadType;

	UPROPERTY(meta = (BindWidget))
	USpinBox* RoadWidthValue;


	UPROPERTY(meta = (BindWidget))
	UScrollableListWidget* WallMaterialList;


	UPROPERTY(meta = (BindWidget))
	UScrollableListWidget* RoadMaterialList;

	UPROPERTY(meta = (BindWidget))
	UScrollableListWidget* FloorMaterialList;

	UPROPERTY(meta = (BindWidget))
	UScrollableListWidget* DoorMaterialList;

	UPROPERTY(meta = (BindWidget))
	UScrollableListWidget* DoorFrameMaterialList;



	virtual void NativeConstruct() override;


	void SwitchToWidget(int32 Index);
};
