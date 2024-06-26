// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataAssets/ItemDataAsset.h"
#include "ItemWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemEntryClicked, FItemInfo, ItemInfo);



UCLASS()
class ARCHVIZ_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()


public:
    UPROPERTY(BlueprintAssignable, Category = "Material Entry")
    FOnItemEntryClicked OnItemEntryClicked;

	UFUNCTION(BlueprintCallable, Category = "Material Entry")
    void SetItemInfo(const FItemInfo& Info);

protected:
    UPROPERTY(meta = (BindWidget))
    class UButton* ItemButton;

    UPROPERTY(meta = (BindWidget))
    class UImage* ItemThumbnail;

    FItemInfo ItemInfo;

	void SetMaterialInfo(const FItemInfo& Info);
    virtual void NativeConstruct() override;

    UFUNCTION()
    void HandleMaterialButtonClicked();


};
