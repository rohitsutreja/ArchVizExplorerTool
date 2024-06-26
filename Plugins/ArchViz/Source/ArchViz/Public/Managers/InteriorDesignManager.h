// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/ItemDataAsset.h"
#include "Managers/ArchVizManager.h"
#include "InteriorDesignManager.generated.h"

class AInteriorActor;
class UInteriorDesignWidget;

UCLASS(Blueprintable)
class ARCHVIZ_API UInteriorDesignManager : public UArchVizManager
{
	GENERATED_BODY()

public:
	virtual void SetUp() override;
	bool IsPlacementValid(AInteriorActor* Actor);
	virtual void End() override;
	virtual void Start() override;

	void CreateAndSelectActor(UStaticMesh* StaticMesh);

	UFUNCTION()
	void OnMeshItemClicked(FItemInfo ItemInfo);


	UPROPERTY()
	UInteriorDesignWidget* InteriorDesignUI;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInteriorDesignWidget> InteriorDesignUIClass;

	UPROPERTY()
	AInteriorActor* SelectedActor;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AInteriorActor> InteriorActorClass;

	bool bIsMovingWithCursor = false;


	void SelectInteriorActorUnderCursor();
	void DeselectCurrentActor();
	void StartActorPlacement();
	void EndActorPlacement();
	void UpdateActorPlacement();
	void DestroySelectedActor();
	bool IsCurrentActorMoving();


	void OnLeftClick();
	void OnRKeyDown();
	void OnMKeyDown();
};
