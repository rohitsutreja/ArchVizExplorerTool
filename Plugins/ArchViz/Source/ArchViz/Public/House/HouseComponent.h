// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchActor.h"
#include "Widgets/PropertyPanelWidget.h"
#include "HouseComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARCHVIZ_API AHouseComponent : public AArchActor
{
	GENERATED_BODY()

public:
	void SnapActorToGrid(const FVector& GridSize);

	void Rotate90Degree();


};
