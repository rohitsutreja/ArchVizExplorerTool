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

	Controller->Notify(TEXT("Wall Created"));

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
		}

		bIsMovingWithCursor = true;
	}
};

void UHouseConstructionManager::EndActorPlacement()
{
	if (SelectedActor->IsA(ADoorActor::StaticClass()))
	{
		auto HitComponent = Controller->GetComponentUnderCursor({ SelectedActor });
		auto HitActor = HitComponent->GetAttachParentActor();

		if (auto Wall = Cast<AWallActor>(HitActor))
		{
			Wall->AttachDoorToComponent(Cast<UStaticMeshComponent>(HitComponent), Cast<ADoorActor>(SelectedActor));
			Wall->UnHighLightBorder();
		}
		else
		{
			Controller->Notify(TEXT("Door can only be placed on wall"));
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



