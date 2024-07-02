// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HouseTemplate.h"
#include "GameFramework/PlayerController.h"
#include "ArchVizController.generated.h"

class UHouseTemplateManager;
class FUniqueIdGenerator;
class UArchVizManager;
class URoadConstructionManager;
class UHouseConstructionManager;
class USaveAndLoadManager;
class UInteriorDesignManager;
class URoadConstructionWidget;
class UHouseConstructionWidget;
class UMainControllerUI;
class AWallActor;
class AFloorActor;
class UInputMappingContext;

UENUM(BlueprintType)
enum class EMode : uint8
{
    RoadConstruction,
    HouseConstruction,
    InteriorDesign,
    SaveMode,
    LoadMode,
    HouseTemplate
};

UCLASS()
class ARCHVIZ_API AArchVizController : public APlayerController
{
    GENERATED_BODY()

public:
    AArchVizController();

    virtual void Tick(float DeltaSeconds) override;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    void InitializeAndSetUpManagers();



    UFUNCTION()
    void InitSaveMode();

    UFUNCTION()
    void InitLoadMode();

    UFUNCTION()
    void InitRoadConstructionMode();

    UFUNCTION()
    void InitHouseConstructionMode();

    UFUNCTION()
    void InitInteriorDesignMode();

    UFUNCTION()
    void InitHouseTemplateMode();



    void CleanUp() const;
    void ClearAllInputMappings() const;

    void SetUpInputForRoadConstructionMode();
    void SetUpInputForHouseConstructionMode();
    void SetUpInputForInteriorDesignMode();
    void SetUpInputForHouseTemplateMode();


    static FSlateBrush GetBrushWithTint(const FLinearColor& Color);
    AHouseTemplate* GetSavedHouseTemplate(const FString& TemplateName);

    AActor* GetActorUnderCursor(const TArray<AActor*>& IgnoredActors = {}) const;
    UPrimitiveComponent* GetComponentUnderCursor(const TArray<AActor*>& IgnoredActors = {}) const;
    FHitResult GetHitResult(const TArray<AActor*>& IgnoredActors = {}) const;


    UFUNCTION(BlueprintImplementableEvent)
    void Notify(const FString& Message);


    EMode GetCurrentMode() const;
    UMainControllerUI* GetMainUI() const;

protected:
    UPROPERTY()
    EMode CurrentMode = EMode::RoadConstruction;


    UPROPERTY(BlueprintReadWrite)
    UMainControllerUI* MainUI;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UMainControllerUI> MainUIClass;


    UPROPERTY()
    UHouseConstructionWidget* HouseConstructionUI;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UHouseConstructionWidget> HouseConstructionUIClass;


    UPROPERTY()
    URoadConstructionManager* RoadConstructionManager;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<URoadConstructionManager> RoadConstructionManagerClass;


    UPROPERTY()
    UHouseConstructionManager* HouseConstructionManager;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UHouseConstructionManager> HouseConstructionManagerClass;


    UPROPERTY()
    UInteriorDesignManager* InteriorDesignManager;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UInteriorDesignManager> InteriorDesignManagerClass;


    UPROPERTY()
    UHouseTemplateManager* HouseTemplateManager;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UHouseTemplateManager> HouseTemplateManagerClass;


    UPROPERTY()
    USaveAndLoadManager* SaveAndLoadManager;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<USaveAndLoadManager> SaveAndLoadManagerClass;


    UPROPERTY()
    UArchVizManager* CurrentManager;

    UPROPERTY()
    UInputMappingContext* RoadConstructionMapping;

    UPROPERTY()
    UInputMappingContext* HouseConstructionMapping;

    UPROPERTY()
    UInputMappingContext* InteriorDesignMapping;


    UPROPERTY()
    UInputMappingContext* HouseTemplateMapping;
};
