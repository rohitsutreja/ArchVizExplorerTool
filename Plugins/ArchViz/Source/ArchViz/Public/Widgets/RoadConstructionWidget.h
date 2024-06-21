// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "RoadConstructionWidget.generated.h"

class USpinBox;
class UHorizontalBox;
class UButton;
/**
 * 
 */
UCLASS()
class ARCHVIZ_API URoadConstructionWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* Mode;
};
