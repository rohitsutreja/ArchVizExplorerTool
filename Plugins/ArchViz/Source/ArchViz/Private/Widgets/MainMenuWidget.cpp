// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainMenuWidget.h"

#include "Components/ScrollBox.h"
#include "SaveAndLoad/ArchVizSave.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SavedProjectList->SetOrientation(Orient_Vertical);
}

void UMainMenuWidget::HandleSlotSelectButtonClicked(const FString& SlotInfo)
{
	if(OnSlotSelectButtonClicked.IsBound())
	{
		OnSlotSelectButtonClicked.Broadcast(SlotInfo);
	}
}

void UMainMenuWidget::HandleSlotDeleteButtonClicked(const FString& SlotInfo)
{
	if (OnSlotDeleteButtonClicked.IsBound())
	{
		OnSlotDeleteButtonClicked.Broadcast(SlotInfo);
	}
}

void UMainMenuWidget::PopulateSlotsList(const TArray<FString>& SlotList)
{
	SavedProjectList->ClearChildren();

	
		for (auto& SlotItem : SlotList)
		{
			auto SlotButton = CreateWidget<USavedSlotItem>(this, SlotItemClass);
			SlotButton->OnSlotSelect.AddDynamic(this, &UMainMenuWidget::HandleSlotSelectButtonClicked);
			SlotButton->OnSlotDelete.AddDynamic(this, &UMainMenuWidget::HandleSlotDeleteButtonClicked);
			SlotButton->SetSlotInfo(SlotItem);
			SavedProjectList->AddChild(SlotButton);
		}
	

}

