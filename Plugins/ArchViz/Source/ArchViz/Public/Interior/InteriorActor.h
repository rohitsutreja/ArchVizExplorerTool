// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchActor.h"
#include "InteriorActor.generated.h"

/**
 * 
 */


UENUM(BlueprintType)
enum class EItemCategory
{
	WallPlaceable,
	FloorPlaceable,
	CeilingPlaceable,
	Table,
	TablePlaceable
};

UCLASS()
class ARCHVIZ_API AInteriorActor : public AArchActor
{
	GENERATED_BODY()


public:
	UPROPERTY()
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY()
	UStaticMesh* CurrentStaticMesh;

	UPROPERTY()
	EItemCategory Category;

	AInteriorActor();

	virtual void HighLightBorder() override;
	virtual void UnHighLightBorder() override;

	virtual void BeginPlay() override;

	void SetStaticMesh(UStaticMesh* StaticMesh);
	void SnapActorToGrid(const FVector& GridSize);
	void Rotate();
};
