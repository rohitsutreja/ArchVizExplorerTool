// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ArchVizController.generated.h"


class UArchVizManager;
class URoadConstructionManager;
class UHouseConstructionManager;
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
	MaterialChanger
};


UCLASS()
class ARCHVIZ_API AArchVizController : public APlayerController
{
	GENERATED_BODY()


public:
	AArchVizController();

	virtual void Tick(float DeltaSeconds) override;

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
	UArchVizManager* CurrentManager;


	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FSlateBrush GetBrushWithTint(const FLinearColor& Color);

	void CleanUp();

	UFUNCTION(BlueprintImplementableEvent)
	void Notify(const FString& Message);


	UFUNCTION()
	void InitRoadConstructionMode();

	UFUNCTION()
	void InitHouseConstructionMode();

	UFUNCTION()
	void InitInteriorDesignMode();

	UFUNCTION()
	void InitMaterialChangeMode();


	AActor* GetActorUnderCursor(const TArray<AActor*>& IgnoredActors = {});
	UPrimitiveComponent* GetComponentUnderCursor(const TArray<AActor*>& IgnoredActors = {});
	FHitResult GetHitResult(const TArray<AActor*>& IgnoredActors = {}) const;


	void SetUpInputForRoadConstructionMode();
	void SetUpInputForHouseConstructionMode();
	void SetUpInputForInteriorDesignMode();


	UPROPERTY()
	UInputMappingContext* RoadConstructionMapping;

	UPROPERTY()
	UInputMappingContext* HouseConstructionMapping;

	UPROPERTY()
	UInputMappingContext* InteriorDesignMapping;

};
