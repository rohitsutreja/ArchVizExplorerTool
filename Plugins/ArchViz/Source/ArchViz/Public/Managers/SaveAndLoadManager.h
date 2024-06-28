// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Managers/ArchVizManager.h"
#include "SaveAndLoadManager.generated.h"

class AWindowActor;
class AInteriorActor;
class ADoorActor;
class AFloorActor;
class AWallActor;
class ARoadSplineActor;
struct FSaveSlotMetadata;
class UMainMenuWidget;
/**
 * 
 */


UCLASS(Blueprintable)
class ARCHVIZ_API USaveAndLoadManager : public UArchVizManager
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnLoadProjectsButtonClicked();

	UFUNCTION()
	void OnNewProjectButtonClicked();

	UFUNCTION()
	void OnSaveProjectButtonClicked();

	UFUNCTION()
	void OnCloseButtonClicked();



	UFUNCTION()
	void OnSlotSelectButtonClicked(const FString& SlotInfo);

	UFUNCTION()
	void OnSlotDeleteButtonClicked(const FString& SlotInfo);


	virtual void SetUp() override;
	virtual void Start() override;
	virtual void End() override;

	void ShowLoadMenu();
	void ShowSaveMenu();

	void SaveSlotMetaData(const FString& SlotName);
	void DeleteSlotMetaData(const FString& SlotInfo);


	TArray<FString> GetSaveSlots();

	void SaveGame(const FString& SlotName);
	void LoadGame(const FString& SlotName);
	void ClearWholeWorld();
	void SynchronizeScrollListUI();

	FString CurrentSlotName = TEXT("");

	bool bFirstTime = true;

protected:
	UPROPERTY()
	UMainMenuWidget* SaveAndLoadUI;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMainMenuWidget> SaveAndLoadUIClass;

	UPROPERTY()
	TArray<FString> SlotsList;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARoadSplineActor> RoadSplineActorClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWallActor> WallActorClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFloorActor> FloorActorClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADoorActor> DoorActorClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWindowActor> WindowActorClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AInteriorActor> InteriorActorClass;
};
