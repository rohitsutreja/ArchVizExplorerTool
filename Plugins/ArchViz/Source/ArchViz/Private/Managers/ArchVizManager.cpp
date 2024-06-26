// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/ArchVizManager.h"

#include "ArchVizController.h"

void UArchVizManager::SetUp()
{
	Controller = Cast<AArchVizController>(GetWorld()->GetFirstPlayerController());
}
