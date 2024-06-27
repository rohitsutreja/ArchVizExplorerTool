// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers//RoadConstructionManager.h"

#include "ArchVizController.h"
#include "Widgets/RoadConstructionWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Roads/RoadSplineActor.h"
#include "Widgets/PropertyPanelWidget.h"


void URoadConstructionManager::CreateNewRoad()
{
	CurrentRoadSpline = GetWorld()->SpawnActor<ARoadSplineActor>(RoadSplineClass, FVector::ZeroVector, FRotator::ZeroRotator);

	Controller->Notify(TEXT("Please Select Points to Create Road"));



	CurrentRoadSpline->OnRoadDelete.BindLambda([this]()
	{
			RoadConstructionUI->SaveButtonText->SetText(FText::FromString(TEXT("Start New Road")));

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

		Controller->Notify(TEXT("Road Saved Successfully."));

	}

	RoadConstructionUI->SaveButtonText->SetText(FText::FromString(TEXT("Start New Road")));

}
void URoadConstructionManager::Start()
{
	Controller->Notify(TEXT("Road Construction Mode Started."));

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
	Super::SetUp();

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
			if(HitResult.GetActor()->IsA(AArchActor::StaticClass()))
			{
				Controller->Notify(TEXT("Please Select Valid Point."));

				return;
			}
			CurrentRoadSpline->AddSplinePoint(HitLocation);
			CurrentRoadSpline->UpdateRoad();
		}
		else
		{
			if(!SelectRoadUnderCursor())
			{
				Controller->Notify(TEXT("Please Start New Road or Select an Existing Road"));
			};
		}
	}
}

void URoadConstructionManager::OnUKeyDown()
{
	if(CurrentRoadSpline)
	{
		if(CurrentRoadSpline->RemoveLastSplinePoint())
		{
			Controller->Notify(TEXT("Undo"));
			CurrentRoadSpline->UpdateRoad();
		}
		else
		{
			Controller->Notify(TEXT("Nothing to undo"));

		}
	}
}


bool URoadConstructionManager::SelectRoadUnderCursor()
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
			return true;
		}
	}

	return false;
}
