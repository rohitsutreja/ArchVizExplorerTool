// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainControllerUI.h"
#include "ArchVizController.h"
#include "Components/Button.h"

void UMainControllerUI::ShowNotification(const FString& Message)
{
	NotificationPanel->SetVisibility(ESlateVisibility::Visible);
	NotificationText->SetText(FText::FromString(Message));

}

void UMainControllerUI::HideNotification()
{
	NotificationPanel->SetVisibility(ESlateVisibility::Collapsed);
}
