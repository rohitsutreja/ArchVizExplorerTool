// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WallActor.h"
#include "House/HouseComponent.h"
#include "DoorActor.generated.h"

/**
 * 
 */
UCLASS()
class ARCHVIZ_API ADoorActor : public AHouseComponent
{
	GENERATED_BODY()


public:

	UPROPERTY()
	UStaticMeshComponent* DoorFrameComponent;

	UPROPERTY()
	UStaticMeshComponent* DoorComponent;


	UPROPERTY(EditAnywhere)
	UStaticMesh* DoorMesh;

	UPROPERTY(EditAnywhere)
	UStaticMesh* DoorFrameMesh;

	ADoorActor();

	void OpenDoor();
	void CloseDoor();


	UFUNCTION()
	void ToggleDoor();

	bool bIsOpen = false;

	UFUNCTION()
	void SetDoorMaterial(FMaterialInfo MaterialInfo);
	UFUNCTION()
	void SetDoorFrameMaterial(FMaterialInfo MaterialInfo);


	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void HighLightBorder() override;
	virtual void UnHighLightBorder() override;


	void DetachFromWall();
};
