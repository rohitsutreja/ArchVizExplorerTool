// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Managers/ArchVizManager.h"
#include "InteriorDesignManager.generated.h"

UCLASS(Blueprintable)
class ARCHVIZ_API UInteriorDesignManager : public UArchVizManager
{
	GENERATED_BODY()

public:
	virtual void SetUp() override;
	virtual void End() override;
	virtual void Start() override;



	
};
