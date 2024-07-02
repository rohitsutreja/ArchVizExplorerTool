// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "House/HouseComponent.h"
#include "SaveAndLoad/ArchVizSave.h"
#include "WindowActor.generated.h"

/**
 * 
 */
UCLASS()
class ARCHVIZ_API AWindowActor : public AHouseComponent
{
	GENERATED_BODY()
public:
	AWindowActor();

	UFUNCTION()
	void OnMaterialChange(FMaterialInfo MaterialInfo);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	virtual void HighLightBorder() override;
	virtual void UnHighLightBorder() override;

	void DetachFromWall();

	UMaterialInterface* GetMaterial();
	void SetMaterial(UMaterialInterface* InMaterial);

	int32 ParentWallComponentIndex;

protected:
	UPROPERTY()
	UStaticMeshComponent* WindowComponent;

	UPROPERTY(EditAnywhere)
	UStaticMesh* WindowMesh;

	UPROPERTY()
	UMaterialInterface* Material;

};
