// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "DataAssets/MaterialDataAsset.h"
#include "ScrollableListWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaterialChange, FMaterialInfo, MaterialInfo);


class UMaterialItemWidget;

UENUM()
enum class EArchItem
{
    Road,
	Wall,
    Floor,
    Door,
};


UCLASS()
class ARCHVIZ_API UScrollableListWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material List")
    UMaterialDataAsset* MaterialDataAsset;

    UFUNCTION(BlueprintCallable, Category = "Material List")
    void PopulateMaterialList();

    UPROPERTY(EditAnywhere)
    TSubclassOf<UMaterialItemWidget> MaterialItemClass;

    UPROPERTY(EditAnywhere)
    TEnumAsByte<EOrientation> Orientation = Orient_Horizontal;

    UPROPERTY(EditAnywhere)
    EArchItem TypeOfMaterial;

    FOnMaterialChange OnMaterialChange;



protected:
    UPROPERTY(meta = (BindWidget))
    class UScrollBox* MaterialScrollBox;

    virtual void NativeConstruct() override;

    UFUNCTION()
    void HandleMaterialItemClicked(FMaterialInfo MaterialInfo);
};
