// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SavedSlotItem.h"
#include "Blueprint/UserWidget.h"
#include "SaveAndLoad/ArchVizSave.h"
#include "MainMenuWidget.generated.h"

class UEditableText;
class UScrollBox;
class UButton;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotSelectButtonClicked, const FString&, SlotInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotDeleteButtonClicked, const FString&, SlotInfo);



UCLASS()
class ARCHVIZ_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleSlotSelectButtonClicked(const FString& SlotInfo);

	UFUNCTION()
	void HandleSlotDeleteButtonClicked(const FString& SlotInfo);

	void PopulateSlotsList(const TArray<FString>& SlotList);
	


public:

	FOnSlotSelectButtonClicked OnSlotSelectButtonClicked;
	FOnSlotDeleteButtonClicked OnSlotDeleteButtonClicked;

	UPROPERTY(meta = (BindWidget))
	UButton* LoadProjectButton;

	UPROPERTY(meta = (BindWidget))
	UButton* NewProjectButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(meta = (BindWidget))
	UEditableText* SaveSlotName;


	UPROPERTY(meta = (BindWidget))
	UScrollBox* SavedProjectList;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MainWidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* LoadWidgetSwitcher;



	UPROPERTY(EditAnywhere)
	TSubclassOf<USavedSlotItem> SlotItemClass;
		
};
