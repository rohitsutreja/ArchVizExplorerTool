// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Managers/ArchVizManager.h"
#include "HouseConstructionManager.generated.h"

class AWindowActor;
class AWallActor;
class AFloorActor;
class ADoorActor;
class AHouseComponent;
class UHouseConstructionWidget;
/**
 * 
 */

UCLASS(Blueprintable)
class ARCHVIZ_API UHouseConstructionManager : public UArchVizManager
{
	GENERATED_BODY()


public:
	UPROPERTY()
	UHouseConstructionWidget* HouseConstructionUI;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHouseConstructionWidget> HouseConstructionUIClass;

	UPROPERTY()
	AHouseComponent* SelectedActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWallActor> WallClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AFloorActor> FloorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ADoorActor> DoorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWindowActor> WindowClass;


	bool bIsNewActor = false;

	bool bIsMovingWithCursor = false;

	bool bIsWallExpanding = false;

	bool bIsFloorExpanding = false;


	virtual void Start() override;

	virtual void SetUp() override;

	virtual void End() override;


	UFUNCTION()
	void CreateAndSelectWall();

	UFUNCTION()
	void CreateAndSelectFloor();

	UFUNCTION()
	void CreateAndSelectDoor();

	UFUNCTION()
	void CreateAndSelectWindow();


	void SelectHouseComponentActorUnderCursor();
	void DeselectCurrentActor();
	void StartActorPlacement();
	void EndActorPlacement();
	void UpdateActorPlacement();
	void DestroySelectedActor();
	bool IsCurrentActorMoving();


	void OnLeftClick();
	void HandleFloorDragging();
	void HandleWallDragging();
	void OnRKeyDown();
	void OnMKeyDown();
};
