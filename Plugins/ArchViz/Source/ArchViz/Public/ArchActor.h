// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/MaterialDataAsset.h"
#include "GameFramework/Actor.h"
#include "ArchActor.generated.h"




class UPropertyPanelWidget;

UCLASS(Abstract)
class ARCHVIZ_API AArchActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AArchActor();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	UPropertyPanelWidget* PropertyPanelUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPropertyPanelWidget> PropertyPanelClass;

	UFUNCTION()
	virtual void OnDeleteButtonClicked();

	void ShowPropertyPanel();
	void HidePropertyPanel();

	virtual void HighLightBorder() PURE_VIRTUAL(AArchActor::HighLightBorder, );
	virtual void UnHighLightBorder() PURE_VIRTUAL(AArchActor::HighLightBorder, );
};
