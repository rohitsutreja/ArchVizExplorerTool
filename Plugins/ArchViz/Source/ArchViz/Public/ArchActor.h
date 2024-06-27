// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
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

	int32 ID;

	UPROPERTY()
	UPropertyPanelWidget* PropertyPanelUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPropertyPanelWidget> PropertyPanelClass;

	UFUNCTION()
	virtual void OnDeleteButtonClicked();

	void ShowPropertyPanel();
	void HidePropertyPanel();

	int32 GetId();

	virtual void HighLightBorder() PURE_VIRTUAL(AArchActor::HighLightBorder, );
	virtual void UnHighLightBorder() PURE_VIRTUAL(AArchActor::HighLightBorder, );


private:
	inline static int32 NextID = 1;

	static int32 GenerateID()
	{
		return NextID++;
	}
};
