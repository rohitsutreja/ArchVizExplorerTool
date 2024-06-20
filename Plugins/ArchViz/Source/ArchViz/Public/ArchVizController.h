// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "House/DoorActor.h"
#include "House/HouseComponent.h"
#include "Roads/RoadSplineActor.h"
#include "Widgets/HouseConstructionWidget.h"
#include "Widgets/MainControllerUI.h"
#include "ArchVizController.generated.h"


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
/**
 * 
 */
UCLASS()
class ARCHVIZ_API AArchVizController : public APlayerController
{
	GENERATED_BODY()


public:
	AArchVizController();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY()
	EMode CurrentMode = EMode::RoadConstruction;

	UPROPERTY()
	UMainControllerUI* MainUI;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMainControllerUI> MainUIClass;

	UPROPERTY()
	UHouseConstructionWidget* HouseConstructionUI;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHouseConstructionWidget> HouseConstructionUIClass;

	UPROPERTY()
	ARoadSplineActor* CurrentRoadSpline;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARoadSplineActor> RoadSplineClass;

	UFUNCTION()
	void CreateAndSelectWall();

	UFUNCTION()
	void OnWallLengthChange(float InValue);

	UFUNCTION()
	void DeleteSelectedActor();

	UFUNCTION()
	void CreateAndSelectFloor();

	UFUNCTION()
	void CreateAndSelectDoor();

	virtual void BeginPlay() override;

	void CleanUp();


	UFUNCTION()
	void InitRoadConstructionMode();

	UFUNCTION()
	void InitHouseConstructionMode();

	UFUNCTION()
	void InitInteriorDesignMode();

	UFUNCTION()
	void InitMaterialChangeMode();



	void OnLeftMouseButtonDown(const FInputActionValue& InputActionValue);

	void SetupInputComponent() override;

	void SetUpInputForRoadConstructionMode();
	void OnRKeyDown(const FInputActionValue& InputActionValue);

	bool IsCurrentActorMoving();
	void SelectHouseComponentActorUnderCursor();
	AActor* GetActorUnderCursor();
	void DeselectCurrentActor();
	USceneComponent* GetComponentUnderCursor();
	void StartActorPlacement();
	void EndActorPlacement();
	void UpdateActorPlacement();
	void OnMKeyDown();

	void SetUpInputForHouseConstructionMode();
	void SetUpInputForInteriorDesignMode();
	void DestroySelectedActor();

	UPROPERTY()
	UInputMappingContext* RoadConstructionMapping;

	UPROPERTY()
	UInputMappingContext* HouseConstructionMapping;

	UPROPERTY()
	UInputMappingContext* InteriorDesignMapping;

	UPROPERTY()
	AHouseComponent* SelectedActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWallActor> WallClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AFloorActor> FloorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ADoorActor> DoorClass;

	bool bIsMovingWithCursor = false;


};
