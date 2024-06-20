// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	UStaticMesh* DoorMesh;


	ADoorActor();


	virtual void BeginPlay() override;

	virtual void HighLightBorder() override;

	virtual void UnHighLightBorder() override;

	virtual void Destroyed() override;

};
