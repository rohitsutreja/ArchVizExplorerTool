// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "SaveAndLoad/ArchVizSave.h"
#include "SavedSlotItem.generated.h"

class UButton;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotSelect, const FString&, SlotInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotDelete, const FString&, SlotInfo);


UCLASS()
class ARCHVIZ_API USavedSlotItem : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION()
	void HandleSlotClick();

	UFUNCTION()
	void HandleSlotDeleteClick();

	virtual void NativeConstruct() override;

	void SetSlotInfo(const FString& InSlotInfo);

	FOnSlotSelect OnSlotSelect;
	FOnSlotDelete OnSlotDelete;

	FString SaveSlotInfo;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SlotName;

	UPROPERTY(meta = (BindWidget))
	UButton* SlotButton;


	UPROPERTY(meta = (BindWidget))
	UButton* DeleteButton;
};
