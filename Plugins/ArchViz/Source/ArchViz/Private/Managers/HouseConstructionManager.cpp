// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/HouseConstructionManager.h"

#include "ArchVizController.h"
#include "Components/Button.h"
#include "House/DoorActor.h"
#include "Widgets/HouseConstructionWidget.h"
#include "House/FloorActor.h"
#include "House/HouseComponent.h"
#include "House/WallActor.h"


void UHouseConstructionManager::Start()
{
	Controller->Notify(TEXT("House Construction Mode Started."));

	if (IsValid(HouseConstructionUI))
	{
		HouseConstructionUI->AddToViewport(0);
	}
}

void UHouseConstructionManager::SetUp()
{
	Super::SetUp();

	HouseConstructionUI = CreateWidget<UHouseConstructionWidget>(GetWorld(), HouseConstructionUIClass);

	if (IsValid(HouseConstructionUI))
	{
		HouseConstructionUI->WallButton->OnClicked.AddDynamic(this, &UHouseConstructionManager::CreateAndSelectWall);
		HouseConstructionUI->FloorButton->OnClicked.AddDynamic(this, &UHouseConstructionManager::CreateAndSelectFloor);
		HouseConstructionUI->DoorButton->OnClicked.AddDynamic(this, &UHouseConstructionManager::CreateAndSelectDoor);
		HouseConstructionUI->WindowButton->OnClicked.AddDynamic(this, &UHouseConstructionManager::CreateAndSelectWindow);

	}
}

void UHouseConstructionManager::End()
{
	if (IsCurrentActorMoving())
	{
		DestroySelectedActor();
	}

	DeselectCurrentActor();

	if (IsValid(HouseConstructionUI))
	{
		HouseConstructionUI->RemoveFromParent();
	}
}

void UHouseConstructionManager::CreateAndSelectWall()
{

	if (IsCurrentActorMoving())
	{
		bIsMovingWithCursor = false;
		SelectedActor->Destroy();
		SelectedActor = nullptr;
	}


	DeselectCurrentActor();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	SelectedActor = GetWorld()->SpawnActor<AWallActor>(WallClass, FVector{ 0, 0, 0 },
		FRotator{ 0, 0, 0 },
		SpawnParams);

	SelectedActor->HighLightBorder();

	Controller->Notify(TEXT("Wall Created. Click to Place on Desired Location."));

	bIsMovingWithCursor = true;
}

void UHouseConstructionManager::CreateAndSelectFloor() {

	if (IsCurrentActorMoving())
	{
		DestroySelectedActor();
	}
	else
	{
		DeselectCurrentActor();
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	SelectedActor = GetWorld()->SpawnActor<AHouseComponent>(FloorClass, FVector{ 0, 0, 0 },
		FRotator{ 0, 0, 0 },
		SpawnParams);

	Controller->Notify(TEXT("Floor Created. Click to Place on Desired Location."));


	SelectedActor->HighLightBorder();

	StartActorPlacement();
}

void UHouseConstructionManager::CreateAndSelectDoor()
{
	if (IsCurrentActorMoving())
	{
		DestroySelectedActor();
	}

	DeselectCurrentActor();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	SelectedActor = GetWorld()->SpawnActor<AHouseComponent>(DoorClass, FVector{ 0, 0, 0 },
		FRotator{ 0, 0, 0 },
		SpawnParams);

	Controller->Notify(TEXT("Door Created. Click on Wall to Place it."));

	SelectedActor->HighLightBorder();

	StartActorPlacement();
}

void UHouseConstructionManager::CreateAndSelectWindow()
{
	if (IsCurrentActorMoving())
	{
		DestroySelectedActor();
	}

	DeselectCurrentActor();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	SelectedActor = GetWorld()->SpawnActor<AHouseComponent>(WindowClass, FVector{ 0, 0, 0 },
		FRotator{ 0, 0, 0 },
		SpawnParams);

	Controller->Notify(TEXT("Window Created. Click on Wall to Place it."));

	SelectedActor->HighLightBorder();

	StartActorPlacement();
}


void UHouseConstructionManager::SelectHouseComponentActorUnderCursor()
{
	AActor* ActorUnderCursor = Controller->GetActorUnderCursor({});


	if (IsValid(ActorUnderCursor))
	{
		if (auto HouseComponentActor = Cast<AHouseComponent>(ActorUnderCursor))
		{
			SelectedActor = HouseComponentActor;

			SelectedActor->HighLightBorder();

			if (IsValid(SelectedActor->PropertyPanelUI))
			{
				SelectedActor->ShowPropertyPanel();
			}
		}
	}
}

void UHouseConstructionManager::DeselectCurrentActor()
{
	if (IsValid(SelectedActor))
	{
		SelectedActor->UnHighLightBorder();

		if (IsValid(SelectedActor->PropertyPanelUI))
		{
			SelectedActor->HidePropertyPanel();
		}
	}

	SelectedActor = nullptr;


};

void UHouseConstructionManager::StartActorPlacement()
{
	if (IsValid(SelectedActor))
	{

		SelectedActor->HidePropertyPanel();

		if (auto Door = Cast<ADoorActor>(SelectedActor))
		{
			Door->DetachFromWall();
			Door->ParentWallComponentIndex = -1;
		}


		if (auto Window = Cast<AWindowActor>(SelectedActor))
		{
			Window->DetachFromWall();
			Window->ParentWallComponentIndex = -1;
		}
		bIsMovingWithCursor = true;
	}
};

void UHouseConstructionManager::EndActorPlacement()
{
	if (SelectedActor->IsA(ADoorActor::StaticClass()) || SelectedActor->IsA(AWindowActor::StaticClass()))
	{
		auto HitComponent = Controller->GetComponentUnderCursor({ SelectedActor });
		auto HitActor = HitComponent->GetAttachParentActor();

		if (auto Wall = Cast<AWallActor>(HitActor))
		{
			if(auto Door = Cast<ADoorActor>(SelectedActor))
			{
				if(!Wall->AttachDoorToComponent(Cast<UStaticMeshComponent>(HitComponent), Door))
				{
					Controller->Notify(TEXT("There is already Window/Door Present at This Location."));
					return;

				};
			}
			else if(auto Window = Cast<AWindowActor>(SelectedActor))
			{
				if(!Wall->AttachWindowToComponent(Cast<UStaticMeshComponent>(HitComponent), Window))
				{
					Controller->Notify(TEXT("There is already Window/Door Present at This Location."));
					return;
				};
			}
			Wall->UnHighLightBorder();
		}
		else
		{
			Controller->Notify(TEXT("Doors and Windows Can Only be Placed on Wall"));
			return;
		}
	}


	bIsMovingWithCursor = false;
};

void UHouseConstructionManager::UpdateActorPlacement()
{
	if(IsValid(SelectedActor))
	{
		const FHitResult HitResult = Controller->GetHitResult({ SelectedActor });

		SelectedActor->SetActorLocation(HitResult.Location);
		SelectedActor->SnapActorToGrid(FVector{ 20 });
	}
}

void UHouseConstructionManager::DestroySelectedActor()
{
	bIsMovingWithCursor = false;
	SelectedActor->Destroy();
	SelectedActor = nullptr;
}

bool UHouseConstructionManager::IsCurrentActorMoving()
{
	return bIsMovingWithCursor;
}

void UHouseConstructionManager::OnLeftClick()
{
	if (IsValid(SelectedActor))
	{
		if (IsCurrentActorMoving())
		{
			EndActorPlacement();

			if (IsValid(SelectedActor->PropertyPanelUI))
			{
				SelectedActor->ShowPropertyPanel();
			}

		}
		else
		{
			if (SelectedActor != Controller->GetActorUnderCursor())
			{
				DeselectCurrentActor();

				SelectHouseComponentActorUnderCursor();
			}
		}
	}
	else
	{
		SelectHouseComponentActorUnderCursor();
	}
};


void UHouseConstructionManager::OnRKeyDown()
{
	if (IsValid(SelectedActor))
	{
		SelectedActor->Rotate();
	}
}

void UHouseConstructionManager::OnMKeyDown()
{
	StartActorPlacement();

};



