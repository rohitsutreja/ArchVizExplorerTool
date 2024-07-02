// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HouseTemplate.h"
#include "Managers/ArchVizManager.h"
#include "HouseTemplateManager.generated.h"


class UHouseTemplateWidget;

UCLASS(Blueprintable)
class ARCHVIZ_API UHouseTemplateManager : public UArchVizManager
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UHouseTemplateWidget> HouseTemplateUIClass;

	UPROPERTY()
	UHouseTemplateWidget* HouseTemplateUI;

	UPROPERTY()
	AHouseTemplate* SelectedHouseTemplate;

	bool bIsMoving = false;

	UFUNCTION()
	void CreateAndSelectThirdHouseTemplate();
	UFUNCTION()
	void CreateAndSelectSecondHouseTemplate();
	UFUNCTION()
	void CreateAndSelectFirstHouseTemplate();

	UFUNCTION()
	void OnSaveButtonClicked();


	virtual void SetUp() override;
	virtual void Start() override;
	virtual void End() override;

	void OnRKeyDown();
	void OnLeftClick();
};
