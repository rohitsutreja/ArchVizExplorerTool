// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PropertyPanelWidget.h"

#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"


void UPropertyPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
}

void UPropertyPanelWidget::SwitchToWidget(int32 Index)
{
	if (IsValid(WidgetSwitcher))
	{
		WidgetSwitcher->SetActiveWidgetIndex(Index);
	}
}

