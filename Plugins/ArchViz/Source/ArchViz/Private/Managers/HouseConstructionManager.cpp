// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/HouseConstructionManager.h"

#include "ArchVizController.h"
#include "VectorTypes.h"
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
		HouseConstructionUI->WallButton->OnClicked.AddUniqueDynamic(this, &UHouseConstructionManager::CreateAndSelectWall);
		HouseConstructionUI->FloorButton->OnClicked.AddUniqueDynamic(this, &UHouseConstructionManager::CreateAndSelectFloor);
		HouseConstructionUI->DoorButton->OnClicked.AddUniqueDynamic(this, &UHouseConstructionManager::CreateAndSelectDoor);
		HouseConstructionUI->WindowButton->OnClicked.AddUniqueDynamic(this, &UHouseConstructionManager::CreateAndSelectWindow);
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

	bIsNewActor = true;

	StartActorPlacement();
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

	bIsNewActor = true;

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

	bIsWallExpanding = false;

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


	bIsNewActor = false;
	bIsFloorExpanding = false;
	bIsWallExpanding = false;

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

		if(bIsNewActor && (SelectedActor->IsA(AWallActor::StaticClass()) || SelectedActor->IsA(AFloorActor::StaticClass())))
		{
			auto Rotation = Controller->PlayerCameraManager->GetCameraRotation();

			auto WallRot = SelectedActor->GetActorRotation();


			if((Rotation.Yaw >=0 && Rotation.Yaw < 45) || (Rotation.Yaw >= 315 && Rotation.Yaw < 360))
			{
				SelectedActor->SetActorRotation(FRotator(0,90,0));
			}
			else if(Rotation.Yaw >= 45 && Rotation.Yaw < 135)
			{
				SelectedActor->SetActorRotation(FRotator(0, 180, 0));

			}
			else if(Rotation.Yaw >= 135 && Rotation.Yaw < 225)
			{
				SelectedActor->SetActorRotation(FRotator(0, -90, 0));

			}
			else
			{
				SelectedActor->SetActorRotation(FRotator(0, 0, 0));
			}
			
		}

		SelectedActor->SetActorLocation(HitResult.Location);
		SelectedActor->SnapActorToGrid(FVector{ 20 });
	}
}


void UHouseConstructionManager::DestroySelectedActor()
{
	bIsNewActor = false;
	bIsMovingWithCursor = false;
	bIsWallExpanding = false;
	bIsFloorExpanding = false;
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

		if (SelectedActor->IsA(AWallActor::StaticClass()))
		{
			if(!IsCurrentActorMoving() && bIsWallExpanding)
			{
				if (bIsNewActor)
				{
					SelectedActor->SyncProperties();
					bIsNewActor = false;
				}
				bIsWallExpanding = false;
				return;
			}
		}

		if (auto Floor = Cast<AFloorActor>(SelectedActor))
		{
			if (!IsCurrentActorMoving() && bIsFloorExpanding)
			{
				if (bIsNewActor)
				{
					Floor->OldDimensions = Floor->GetDimensions();
					SelectedActor->SyncProperties();
					bIsNewActor = false;
				}
				bIsFloorExpanding = false;

				return;
			}
		}

		if (IsCurrentActorMoving())
		{
			EndActorPlacement();

			if(SelectedActor->IsA(AWallActor::StaticClass()) && bIsNewActor)
			{
				bIsWallExpanding = true;
				Controller->Notify(TEXT("Drag Your Mouse To Extend the Wall."));

			}

			if (SelectedActor->IsA(AFloorActor::StaticClass()) && bIsNewActor)
			{
				bIsFloorExpanding = true;
				Controller->Notify(TEXT("Drag Your Mouse To Extend the Floor."));

			}

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


void UHouseConstructionManager::HandleFloorDragging()
{
	if (auto Floor = Cast<AFloorActor>(SelectedActor))
	{
		if (bIsFloorExpanding && bIsNewActor)
		{
			FVector StartPoint = Floor->GetActorLocation();

			FHitResult HitResult = Controller->GetHitResult({ SelectedActor });
		
			HitResult.Location = AArchActor::GetSnappedLocation(HitResult.Location, FVector{20});

			if (Floor->EndPoint == HitResult.Location) {
				return;
			}

			Floor->EndPoint = HitResult.Location;

			double EdgeOffset{ 10.0f };

			double XDistance = Floor->EndPoint.X - StartPoint.X;
			double YDistance = Floor->EndPoint.Y - StartPoint.Y;
			double ZDistance = 20.0;

			auto Dimensions = FVector{ abs(XDistance) + (2 * EdgeOffset), abs(YDistance) + (2 * EdgeOffset), ZDistance };

		/*	Dimensions.X = static_cast<int32>(Dimensions.X) / 200 * 200 + 20;
			Dimensions.Y = static_cast<int32>(Dimensions.Y) / 200 * 200 + 20;*/

			Floor->SetDimensions(Dimensions);

			Floor->AdjustOffset();

			Floor->GenerateFloor();
		}
	}
}
void UHouseConstructionManager::HandleWallDragging()
{
	if (auto Wall = Cast<AWallActor>(SelectedActor))
	{
		if (bIsWallExpanding && bIsNewActor)
		{
			FVector StartPoint = Wall->GetActorLocation();

			const FHitResult HitResult = Controller->GetHitResult({ SelectedActor });

			auto HitLocation = HitResult.Location;

			auto XDiff = HitLocation.X - StartPoint.X;
			auto YDiff = HitLocation.Y - StartPoint.Y;

			if (abs(XDiff) >= abs(YDiff)) {

				Wall->SetLength(abs(XDiff));

				if (XDiff >= 0) {
					Wall->SetActorRotation(FRotator{ 0.0 });
				}
				else {
					Wall->SetActorRotation(FRotator{ 0.0, 180.0, 0.0 });
				}
			}
			else {

				Wall->SetLength(abs(YDiff));

				if (YDiff >= 0) {
					Wall->SetActorRotation(FRotator{ 0.0, 90.0, 0.0 });
				}
				else {
					Wall->SetActorRotation(FRotator{ 0.0, -90, 0.0 });
				}
			}
		}
	}
	
}


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



