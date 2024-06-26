// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataAssets/ItemDataAsset.h"
#include "ItemsList.generated.h"


class UItemWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemChange, FItemInfo, ItemInfo);




UCLASS()
class ARCHVIZ_API UItemsList : public UUserWidget
{
	GENERATED_BODY()
public:


    FOnItemChange OnItemChange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material List")
    UItemDataAsset* ItemDataAsset;

    UFUNCTION(BlueprintCallable, Category = "Material List")
    void PopulateItemsList();

    UPROPERTY(EditAnywhere)
    TSubclassOf<UItemWidget> ItemWidgetClass;

    UPROPERTY(EditAnywhere)
    TEnumAsByte<EOrientation> Orientation = Orient_Horizontal;



protected:
    UPROPERTY(meta = (BindWidget))
    class UScrollBox* ItemScrollBox;

    virtual void NativeConstruct() override;

    UFUNCTION()
    void HandleItemClicked(FItemInfo ItemInfo);
};
