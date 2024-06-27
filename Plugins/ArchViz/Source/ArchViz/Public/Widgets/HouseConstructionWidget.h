// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HouseConstructionWidget.generated.h"

class UVerticalBox;
class UGridPanel;
class UWidgetSwitcher;
class UButton;
/**
 * 
 */
UCLASS()
class ARCHVIZ_API UHouseConstructionWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(meta = (BindWidget))
	UButton* WallButton;

	UPROPERTY(meta = (BindWidget))
	UButton* FloorButton;

	UPROPERTY(meta = (BindWidget))
	UButton* DoorButton;


	UPROPERTY(meta = (BindWidget))
	UButton* WindowButton;



	virtual void NativeConstruct() override;


};
