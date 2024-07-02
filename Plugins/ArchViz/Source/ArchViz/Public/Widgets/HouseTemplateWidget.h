// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "HouseTemplateWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class ARCHVIZ_API UHouseTemplateWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(meta = (BindWidget))
	UButton* FirstHouseButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SecondHouseButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ThirdHouseButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton;

	UPROPERTY(meta = (BindWidget))
	UBorder* PropertyPanelBox;

	UPROPERTY(meta = (BindWidget))
	UBorder* TemplateList;

	void ShowPanel();
	void HidePanel();

};
