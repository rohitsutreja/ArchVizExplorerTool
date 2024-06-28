// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SavedSlotItem.h"

#include "Components/Button.h"

void USavedSlotItem::HandleSlotClick()
{
	OnSlotSelect.Broadcast(SaveSlotInfo);
}

void USavedSlotItem::HandleSlotDeleteClick()
{
	OnSlotDelete.Broadcast(SaveSlotInfo);
}

void USavedSlotItem::NativeConstruct()
{
	Super::NativeConstruct();
	SlotName->SetText(FText::FromString(SaveSlotInfo));
	SlotButton->OnClicked.AddDynamic(this, &USavedSlotItem::HandleSlotClick);
	DeleteButton->OnClicked.AddDynamic(this, &USavedSlotItem::HandleSlotDeleteClick);
}

void USavedSlotItem::SetSlotInfo(const FString& InSlotInfo)
{
	SaveSlotInfo = InSlotInfo;
}
