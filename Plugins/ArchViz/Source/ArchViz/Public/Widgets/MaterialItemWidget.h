#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataAssets/MaterialDataAsset.h"
#include "MaterialItemWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaterialEntryClicked, FMaterialInfo, MaterialInfo);

UCLASS()
class ARCHVIZ_API UMaterialItemWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "Material Entry")
    FOnMaterialEntryClicked OnMaterialEntryClicked;

    UFUNCTION(BlueprintCallable, Category = "Material Entry")
    void SetMaterialInfo(const FMaterialInfo& Info);

protected:
    UPROPERTY(meta = (BindWidget))
    class UButton* MaterialButton;


    UPROPERTY(meta = (BindWidget))
    class UImage* MaterialThumbnail;


    FMaterialInfo MaterialInfo;

    virtual void NativeConstruct() override;

    UFUNCTION()
    void HandleMaterialButtonClicked();
};
