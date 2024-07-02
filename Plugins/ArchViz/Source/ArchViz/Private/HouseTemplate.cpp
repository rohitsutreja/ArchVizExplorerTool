// Fill out your copyright notice in the Description page of Project Settings.


#include "HouseTemplate.h"

#include "ArchVizController.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/SaveAndLoadManager.h"
#include "SaveAndLoad/ArchVizSave.h"


AHouseTemplate::AHouseTemplate()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;


}


void AHouseTemplate::SnapActorToGrid(const FVector& GridSize)
{
	FVector ActorLocation = GetActorLocation();

	ActorLocation.X = FMath::RoundToFloat(ActorLocation.X / GridSize.X) * GridSize.X;
	ActorLocation.Y = FMath::RoundToFloat(ActorLocation.Y / GridSize.Y) * GridSize.Y;
	ActorLocation.Z = FMath::RoundToFloat(ActorLocation.Z / GridSize.Z) * GridSize.Z;

	SetActorLocation(ActorLocation);
}



