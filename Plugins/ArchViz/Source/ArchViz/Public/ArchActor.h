// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArchActor.generated.h"

UCLASS()
class ARCHVIZ_API AArchActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AArchActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void HighLightBorder();
	virtual void UnHighLightBorder();
};
