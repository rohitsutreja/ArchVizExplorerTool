// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include  "Components/Border.h"
#include "MainControllerUI.generated.h"

class UBorder;
class UButton;
/**
 *
 */
UCLASS()
class ARCHVIZ_API UMainControllerUI : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(meta = (BindWidget))
	UButton* RoadButton;
	UPROPERTY(meta = (BindWidget))
	UButton* HouseButton;
	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton;
	UPROPERTY(meta = (BindWidget))
	UButton* InteriorButton;

	UPROPERTY(meta = (BindWidget))
	UBorder* RoadButtonBorder;
	UPROPERTY(meta = (BindWidget))
	UBorder* HouseButtonBorder;
	UPROPERTY(meta = (BindWidget))
	UBorder* SaveButtonBorder;
	UPROPERTY(meta = (BindWidget))
	UBorder* InteriorButtonBorder;


	UPROPERTY(meta =(BindWidget))
	UTextBlock* NotificationText;

	UPROPERTY(meta = (BindWidget))
	UBorder* NotificationPanel;

	UFUNCTION(BlueprintCallable)
	void ShowNotification(const FString& Message);

	UFUNCTION(BlueprintCallable)
	void HideNotification();
};
