// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers//RoadConstructionManager.h"

#include "ArchVizController.h"
#include "Widgets/RoadConstructionWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Roads/RoadSplineActor.h"


void URoadConstructionManager::CreateNewRoad()
{
	CurrentRoadSpline = GetWorld()->SpawnActor<ARoadSplineActor>(RoadSplineClass, FVector::ZeroVector, FRotator::ZeroRotator);

	CurrentRoadSpline->OnRoadDelete.BindLambda([this]()
	{
			RoadConstructionUI->SaveButtonText->SetText(FText::FromString(TEXT("New Road")));

	});
	ArrayOfRoads.Add(CurrentRoadSpline);

	CurrentRoadSpline->ShowPropertyPanel();

	RoadConstructionUI->SaveButtonText->SetText(FText::FromString(TEXT("Save Road")));
}


void URoadConstructionManager::SaveCurrentRoad()
{
	if (IsValid(CurrentRoadSpline))
	{
		CurrentRoadSpline->PropertyPanelUI->RemoveFromParent();

		CurrentRoadSpline->UnHighLightBorder();

		CurrentRoadSpline = nullptr;
	}

	RoadConstructionUI->SaveButtonText->SetText(FText::FromString(TEXT("New Road")));

}
void URoadConstructionManager::Start()
{
	if (IsValid(RoadConstructionUI))
	{
		RoadConstructionUI->AddToViewport(0);
	}
}

void URoadConstructionManager::HandleButtonClick()
{

	if (IsValid(CurrentRoadSpline))
	{
		SaveCurrentRoad();
	}
	else
	{
		CreateNewRoad();
	}

}

void URoadConstructionManager::SetUp()
{
	Controller = Cast<AArchVizController>(GetWorld()->GetFirstPlayerController());

	RoadConstructionUI = CreateWidget<URoadConstructionWidget>(GetWorld(), RoadConstructionUIClass);

	if (IsValid(RoadConstructionUI))
	{
		RoadConstructionUI->SaveButton->OnClicked.AddDynamic(this, &URoadConstructionManager::HandleButtonClick);
	}
}

void URoadConstructionManager::End()
{
	if (IsValid(RoadConstructionUI))
	{
		RoadConstructionUI->RemoveFromParent();
	}

	SaveCurrentRoad();
}


void URoadConstructionManager::OnLeftClick()
{
	
	FHitResult HitResult = Controller->GetHitResult({ CurrentRoadSpline });


	if (HitResult.bBlockingHit)
	{
		FVector HitLocation = HitResult.Location;

		if (IsValid(CurrentRoadSpline))
		{
			CurrentRoadSpline->AddSplinePoint(HitLocation);
			CurrentRoadSpline->UpdateRoad();
		}
		else
		{
			SelectRoadUnderCursor();
			
		}
	}
}


void URoadConstructionManager::SelectRoadUnderCursor()
{
	if(auto ActorUnderCursor = Controller->GetActorUnderCursor({}))
	{
		if(ActorUnderCursor->IsA(ARoadSplineActor::StaticClass()))
		{
			CurrentRoadSpline = Cast<ARoadSplineActor>(ActorUnderCursor);
			CurrentRoadSpline->PropertyPanelUI->AddToViewport();
			CurrentRoadSpline->HighLightBorder();

			if(IsValid(RoadConstructionUI))
			{
				RoadConstructionUI->SaveButtonText->SetText(FText::FromString(TEXT("Save Road")));
			}
		}
	}
}