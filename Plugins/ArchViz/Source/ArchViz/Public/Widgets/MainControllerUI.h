// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainControllerUI.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class ARCHVIZ_API UMainControllerUI : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY()
	UButton* RoadButton;
	UPROPERTY()
	UButton* HouseButton;
	UPROPERTY()
	UButton* MaterialButton;
	UPROPERTY()
	UButton* InteriorButton;



};
