// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HouseTemplate.generated.h"

UCLASS()
class ARCHVIZ_API AHouseTemplate : public AActor
{
	GENERATED_BODY()
	
public:	
	AHouseTemplate();

	void SnapActorToGrid(const FVector& GridSize);

	UPROPERTY()
	USceneComponent* SceneRoot;
};
