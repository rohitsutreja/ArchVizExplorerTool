// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HouseTemplateWidget.h"

void UHouseTemplateWidget::ShowPanel()
{
	if(IsValid(PropertyPanelBox))
	{
		PropertyPanelBox->SetVisibility(ESlateVisibility::Visible);
	}
	if(IsValid(TemplateList))
	{
		TemplateList->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UHouseTemplateWidget::HidePanel()
{
	if (IsValid(PropertyPanelBox))
	{
		PropertyPanelBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (IsValid(TemplateList))
	{
		TemplateList->SetVisibility(ESlateVisibility::Visible);
	}
}
