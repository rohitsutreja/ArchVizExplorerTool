// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/SaveAndLoadManager.h"

#include "ArchVizController.h"
#include "EngineUtils.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "House/FloorActor.h"
#include "House/WallActor.h"
#include "House/WindowActor.h"
#include "Widgets/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "SaveAndLoad/ArchVizSave.h"
#include "Widgets/MainControllerUI.h"

void USaveAndLoadManager::OnLoadProjectsButtonClicked()
{
	SynchronizeScrollListUI();
    SaveAndLoadUI->LoadWidgetSwitcher->SetActiveWidgetIndex(1);
}

void USaveAndLoadManager::OnNewProjectButtonClicked()
{
	ClearWholeWorld();

	CurrentSlotName.Empty();

	Controller->Notify(TEXT("New Project is Created."));

	End();
}

void USaveAndLoadManager::OnSlotSelectButtonClicked(const FString& SlotInfo)
{
	if(CurrentSlotName == SlotInfo)
	{
		End();
		Controller->Notify("Project is Already Opened.");
		return;

	}

    LoadGame(SlotInfo);

	CurrentSlotName = SlotInfo;

	End();

    Controller->Notify("Project " + CurrentSlotName +" Loaded Successfully.");

}

void USaveAndLoadManager::OnCloseButtonClicked()
{
	End();
}

void USaveAndLoadManager::OnSlotDeleteButtonClicked(const FString& SlotInfo)
{

	if (SlotInfo == CurrentSlotName)
	{
		Controller->Notify(TEXT("Can Not Delete Running Project."));
		return;
	}

	if(UGameplayStatics::DoesSaveGameExist(SlotInfo,0))
	{
		if (UGameplayStatics::DeleteGameInSlot(SlotInfo, 0))
		{
			DeleteSlotMetaData(SlotInfo);
			
			Controller->Notify(TEXT("Project " + SlotInfo + " Deleted"));
		}
		SynchronizeScrollListUI();
	}
	
	

}

void USaveAndLoadManager::OnSaveProjectButtonClicked()
{
    FString SlotName = SaveAndLoadUI->SaveSlotName->GetText().ToString();

	if(SlotName.IsEmpty())
	{
		Controller->Notify(TEXT("Please Enter Valid Name."));
		return;
	}

	if(GetSaveSlots().Contains(SlotName))
	{
		Controller->Notify(TEXT("This Name Already Exists. Please Choose Differnet Name"));
		return;
	}

	CurrentSlotName = SlotName;

	SaveGame(SlotName);

    Controller->Notify("Project " + CurrentSlotName +" Saved Successfully.");

    SaveSlotMetaData(SlotName);

	SynchronizeScrollListUI();

	SaveAndLoadUI->SaveSlotName->SetText(FText());

	End();
}




void USaveAndLoadManager::SetUp()
{
	Super::SetUp();

    SlotsList = GetSaveSlots();

    SaveAndLoadUI = CreateWidget<UMainMenuWidget>(GetWorld(), SaveAndLoadUIClass);

	if(IsValid(SaveAndLoadUI))
	{
		SaveAndLoadUI->LoadProjectButton->OnClicked.AddDynamic(this, &USaveAndLoadManager::OnLoadProjectsButtonClicked);
		SaveAndLoadUI->NewProjectButton->OnClicked.AddDynamic(this, &USaveAndLoadManager::OnNewProjectButtonClicked);
		SaveAndLoadUI->SaveButton->OnClicked.AddDynamic(this, &USaveAndLoadManager::OnSaveProjectButtonClicked);
		SaveAndLoadUI->CloseButton->OnClicked.AddDynamic(this, &USaveAndLoadManager::OnCloseButtonClicked);
		SaveAndLoadUI->OnSlotSelectButtonClicked.AddDynamic(this, &USaveAndLoadManager::OnSlotSelectButtonClicked);
		SaveAndLoadUI->OnSlotDeleteButtonClicked.AddDynamic(this, &USaveAndLoadManager::OnSlotDeleteButtonClicked);

	}
   
	CurrentSlotName.Empty();

}

void USaveAndLoadManager::Start()
{
    if (IsValid(SaveAndLoadUI))
    {
        SaveAndLoadUI->AddToViewport(1);
    }

	if(Controller->GetCurrentMode() == EMode::SaveMode)
	{
		ShowSaveMenu();
	}
	else
	{
		ShowLoadMenu();
	}


	if (bFirstTime)
	{
		bFirstTime = false;
	}
}

void USaveAndLoadManager::End()
{
	if (IsValid(SaveAndLoadUI))
		SaveAndLoadUI->RemoveFromParent();

	if (IsValid(Controller->GetMainUI()))
	{
		Controller->GetMainUI()->SaveButtonBorder->SetBrushColor(FColor::Black);
		Controller->GetMainUI()->MenuButtonBorder->SetBrushColor(FColor::Black);
	}
}


void USaveAndLoadManager::ShowLoadMenu() const
{
    if (IsValid(SaveAndLoadUI))
    {

		Controller->GetMainUI()->SaveButtonBorder->SetBrushColor(FColor::Green);
		Controller->GetMainUI()->MenuButtonBorder->SetBrushColor(FColor::Green);

        SaveAndLoadUI->AddToViewport(1);
		SaveAndLoadUI->LoadWidgetSwitcher->SetActiveWidgetIndex(0);
        SaveAndLoadUI->MainWidgetSwitcher->SetActiveWidgetIndex(0);

		if(bFirstTime)
		{
			SaveAndLoadUI->CloseButton->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			SaveAndLoadUI->CloseButton->SetVisibility(ESlateVisibility::Visible);
		}
    }
}

void USaveAndLoadManager::ShowSaveMenu()
{
    if (IsValid(SaveAndLoadUI))
    {

		if(CurrentSlotName.IsEmpty())
		{
			Controller->GetMainUI()->SaveButtonBorder->SetBrushColor(FColor::Green);
			Controller->GetMainUI()->MenuButtonBorder->SetBrushColor(FColor::Green);

			SaveAndLoadUI->AddToViewport(1);
			SaveAndLoadUI->MainWidgetSwitcher->SetActiveWidgetIndex(1);
			SaveAndLoadUI->CloseButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Controller->Notify(TEXT("Saved Successfully."));
			End();
			SaveGame(CurrentSlotName);
		}
       
    }
}


void USaveAndLoadManager::SaveSlotMetaData(const FString& SlotName)
{
    UArchVizSave* MetadataInstance = Cast<UArchVizSave>(UGameplayStatics::LoadGameFromSlot("Metadata", 0));

    if (!MetadataInstance)
    {
        MetadataInstance = Cast<UArchVizSave>(UGameplayStatics::CreateSaveGameObject(UArchVizSave::StaticClass()));
    }

    MetadataInstance->SaveMetaDataArray.Add(SlotName);

    UGameplayStatics::SaveGameToSlot(MetadataInstance, "Metadata", 0);
}

void USaveAndLoadManager::DeleteSlotMetaData(const FString& SlotInfo)
{
	UArchVizSave* MetadataInstance = Cast<UArchVizSave>(UGameplayStatics::LoadGameFromSlot("Metadata", 0));

	if (!MetadataInstance)
	{
		return;
	}

	
	MetadataInstance->SaveMetaDataArray.Remove(SlotInfo);

	SlotsList = MetadataInstance->SaveMetaDataArray;

	UGameplayStatics::SaveGameToSlot(MetadataInstance, "Metadata", 0);
}

TArray<FString> USaveAndLoadManager::GetSaveSlots()
{
    UArchVizSave* MetadataInstance = Cast<UArchVizSave>(UGameplayStatics::LoadGameFromSlot("Metadata", 0));

    if (MetadataInstance)
    {
        return MetadataInstance->SaveMetaDataArray;
    }

    return TArray<FString>();
}



void USaveAndLoadManager::SaveGame(const FString& SlotName)
{
	UArchVizSave* SaveGameInstance = Cast<UArchVizSave>(UGameplayStatics::CreateSaveGameObject(UArchVizSave::StaticClass()));

	for (TActorIterator<ARoadSplineActor> It(GetWorld()); It; ++It)
	{
		ARoadSplineActor* RoadActor = *It;
		FRoad RoadData;
		RoadData.ID = RoadActor->GetId();
		RoadData.Transform = RoadActor->GetActorTransform();
		RoadData.SplinePoints = RoadActor->GetSplinePoints();
		RoadData.Type = RoadActor->GetTypeOfRoad();
		RoadData.Width = RoadActor->GetWidth();
		RoadData.Material = RoadActor->GetMaterial();
		if (IsValid(RoadActor->GetAttachParentActor()))
		{
			if (auto ParentActor = Cast<AArchActor>(RoadActor->GetAttachParentActor()))
			{
				RoadData.ParentActorId = ParentActor->GetId();
			}
		}
		else
		{
			RoadData.ParentActorId = -1;
		}
		SaveGameInstance->RoadActorArray.Add(RoadData);
	}


	for (TActorIterator<AWallActor> It(GetWorld()); It; ++It)
	{
		AWallActor* WallActor = *It;
		FWall WallData;
		WallData.ID = WallActor->GetId();
		WallData.Transform = WallActor->GetActorTransform();
		WallData.NumberOfWallSegments = WallActor->GetNumberOfWallSegments();
		WallData.Material = WallActor->GetMaterial();

		if (IsValid(WallActor->GetAttachParentActor()))
		{
			if (auto ParentActor = Cast<AArchActor>(WallActor->GetAttachParentActor()))
			{
				WallData.ParentActorId = ParentActor->GetId();
			}
		}
		else
		{
			WallData.ParentActorId = -1;

		}

		SaveGameInstance->WallActorArray.Add(WallData);
	}

	for (TActorIterator<AFloorActor> It(GetWorld()); It; ++It)
	{
		AFloorActor* FloorActor = *It;
		FFloor FloorData;
		FloorData.ID = FloorActor->GetId();

		FloorData.Transform = FloorActor->GetActorTransform();
		FloorData.BottomMaterial = FloorActor->GetBottomMaterial();
		FloorData.TopMaterial = FloorActor->GetTopMaterial();
		FloorData.Dimensions = FloorActor->GetDimensions();
		if (IsValid(FloorActor->GetAttachParentActor()))
		{
			if (auto ParentActor = Cast<AArchActor>(FloorActor->GetAttachParentActor()))
			{
				FloorData.ParentActorId = ParentActor->GetId();
			}
		}
		else
		{
			FloorData.ParentActorId = -1;

		}
		SaveGameInstance->FloorActorArray.Add(FloorData);
	}


	for (TActorIterator<ADoorActor> It(GetWorld()); It; ++It)
	{
		ADoorActor* DoorActor = *It;
		FDoor DoorData;
		DoorData.ID = DoorActor->GetId();
		DoorData.Transform = DoorActor->GetActorTransform();
		DoorData.DoorMaterial = DoorActor->GetDoorMaterial();
		DoorData.FrameMaterial = DoorActor->GetDoorFrameMaterial();
		DoorData.bIsOpen = DoorActor->IsDoorOpen();
		DoorData.ParentComponentIndex = DoorActor->ParentWallComponentIndex;

		if (IsValid(DoorActor->GetAttachParentActor()))
		{
			if (auto ParentActor = Cast<AArchActor>(DoorActor->GetAttachParentActor()))
			{
				DoorData.ParentActorId = ParentActor->GetId();
			}
		}
		else
		{
			DoorData.ParentActorId = -1;

		}
		SaveGameInstance->DoorActorArray.Add(DoorData);
	}


	for (TActorIterator<AInteriorActor> It(GetWorld()); It; ++It)
	{
		AInteriorActor* InteriorActor = *It;
		FInterior InteriorData;
		InteriorData.ID = InteriorActor->GetId();
		InteriorData.Transform = InteriorActor->GetActorTransform();
		InteriorData.StaticMesh = InteriorActor->GetCurrentStaticMesh();
		if(IsValid(InteriorActor->GetAttachParentActor()))
		{
			if(auto ParentActor = Cast<AArchActor>(InteriorActor->GetAttachParentActor()))
			{
				InteriorData.ParentActorId = ParentActor->GetId();
			}
		}
		else
		{
			InteriorData.ParentActorId = -1;
		}
		SaveGameInstance->InteriorActorArray.Add(InteriorData);
	}


	for (TActorIterator<AWindowActor> It(GetWorld()); It; ++It)
	{
		AWindowActor* WindowActor = *It;
		FWindow WindowData;
		WindowData.ID = WindowActor->GetId();
		WindowData.Transform = WindowActor->GetActorTransform();
		WindowData.WindowMaterial = WindowActor->GetMaterial();
		WindowData.ParentComponentIndex = WindowActor->ParentWallComponentIndex;
		if (IsValid(WindowActor->GetAttachParentActor()))
		{
			if (auto ParentActor = Cast<AArchActor>(WindowActor->GetAttachParentActor()))
			{
				WindowData.ParentActorId = ParentActor->GetId();
			}
		}
		else
		{
			WindowData.ParentActorId = -1;

		}
		SaveGameInstance->WindowActorArray.Add(WindowData);
	}



	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);
}

void USaveAndLoadManager::LoadGame(const FString& SlotName)
{
	UArchVizSave* LoadGameInstance = Cast<UArchVizSave>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

	

	if (LoadGameInstance)
	{

		ClearWholeWorld();


		TMap<int32, AActor*> IdToActorMap;
		TMap<AActor*, int32> ActorToParentActorIdMap;

		for (const FRoad& RoadData : LoadGameInstance->RoadActorArray)
		{
			ARoadSplineActor* RoadActor = GetWorld()->SpawnActor<ARoadSplineActor>(RoadSplineActorClass, RoadData.Transform);
			RoadActor->SetActorTransform(RoadData.Transform);
			RoadActor->SetSplinePoints(RoadData.SplinePoints);
			RoadActor->SetTypeOfRoad(RoadData.Type);
			RoadActor->SetWidth(RoadData.Width);
			RoadActor->SetMaterial(RoadData.Material);
			RoadActor->SynchronizePropertyPanel();
			RoadActor->UpdateRoad();
			RoadActor->UnHighLightBorder();
			IdToActorMap.Add(RoadData.ID, RoadActor);

			if (RoadData.ParentActorId != -1)
			{
				ActorToParentActorIdMap.Add(RoadActor, RoadData.ParentActorId);
			}
		}


		for (const FWall& WallData : LoadGameInstance->WallActorArray)
		{
			AWallActor* WallActor = GetWorld()->SpawnActor<AWallActor>(WallActorClass, WallData.Transform);
			WallActor->SetActorTransform(WallData.Transform);
			WallActor->SetNumberOfWallSegments(WallData.NumberOfWallSegments);
			WallActor->SetMaterial(WallData.Material);
			WallActor->UpdateWall();
			WallActor->SynchronizePropertyPanel();
			WallActor->UnHighLightBorder();
			IdToActorMap.Add(WallData.ID, WallActor);

			if (WallData.ParentActorId != -1)
			{
				ActorToParentActorIdMap.Add(WallActor, WallData.ParentActorId);
			}
		}

		for (const FFloor& FloorData : LoadGameInstance->FloorActorArray)
		{
			AFloorActor* FloorActor = GetWorld()->SpawnActor<AFloorActor>(FloorActorClass, FloorData.Transform);
			FloorActor->SetActorTransform(FloorData.Transform);
			FloorActor->SetDimensions(FloorData.Dimensions);
			FloorActor->SetTopMaterial(FloorData.TopMaterial);
			FloorActor->SetBottomMaterial(FloorData.BottomMaterial);
			FloorActor->SynchronizePropertyPanel();
			FloorActor->GenerateFloor();
			IdToActorMap.Add(FloorData.ID, FloorActor);

			if (FloorData.ParentActorId != -1)
			{
				ActorToParentActorIdMap.Add(FloorActor, FloorData.ParentActorId);
			}
		}


		for (const FDoor& DoorData : LoadGameInstance->DoorActorArray)
		{
			ADoorActor* DoorActor = GetWorld()->SpawnActor<ADoorActor>(DoorActorClass, DoorData.Transform);
			DoorActor->SetActorTransform(DoorData.Transform);
			DoorActor->SetDoorMaterial(DoorData.DoorMaterial);
			DoorActor->SetDoorFrameMaterial(DoorData.FrameMaterial);
			DoorActor->ParentWallComponentIndex = DoorData.ParentComponentIndex;
			DoorData.bIsOpen ? DoorActor->OpenDoor() : DoorActor->CloseDoor();
			DoorActor->SynchronizePropertyPanel();
			IdToActorMap.Add(DoorData.ID, DoorActor);

			if (DoorData.ParentActorId != -1)
			{
				ActorToParentActorIdMap.Add(DoorActor, DoorData.ParentActorId);
			}
		}



		for (const FInterior& InteriorData : LoadGameInstance->InteriorActorArray)
		{
			AInteriorActor* InteriorActor = GetWorld()->SpawnActor<AInteriorActor>(InteriorActorClass, InteriorData.Transform);
			InteriorActor->SetActorTransform(InteriorData.Transform);
			InteriorActor->SetStaticMesh(InteriorData.StaticMesh);
			IdToActorMap.Add(InteriorData.ID, InteriorActor);

			if (InteriorData.ParentActorId != -1)
			{
				ActorToParentActorIdMap.Add(InteriorActor, InteriorData.ParentActorId);
			}
		}


		for (const FWindow& WindowData : LoadGameInstance->WindowActorArray)
		{
			AWindowActor* WindowActor = GetWorld()->SpawnActor<AWindowActor>(WindowActorClass, WindowData.Transform);
			WindowActor->SetActorTransform(WindowData.Transform);
			IdToActorMap.Add(WindowData.ID, WindowActor);

			if (WindowData.ParentActorId != -1)
			{
				ActorToParentActorIdMap.Add(WindowActor, WindowData.ParentActorId);
			}
		}


		for (auto& [Actor, ParentActorId] : ActorToParentActorIdMap)
		{
			if (auto ParentActorPtr = IdToActorMap.Find(ParentActorId))
			{
				if (auto ParentActor = *ParentActorPtr; IsValid(ParentActor))
				{
					if (Actor->IsA(ADoorActor::StaticClass()))
					{
						if (auto ParentWall = Cast<AWallActor>(ParentActor); IsValid(ParentWall))
						{
							auto Door = Cast<ADoorActor>(Actor);
							if(IsValid(Door))
							{
								ParentWall->AddDoorAtIndex(Door->ParentWallComponentIndex, Door);
								ParentWall->UpdateWall();
								ParentWall->UnHighLightBorder();
							}
							
						}
					}
					else if (Actor->IsA(AWindowActor::StaticClass()))
					{
						if (auto ParentWall = Cast<AWallActor>(ParentActor); IsValid(ParentWall))
						{
							auto Window = Cast<AWindowActor>(Actor);
							if (IsValid(Window))
							{
								ParentWall->AttachWindowAtIndex(Window->ParentWallComponentIndex, Window);
								ParentWall->UpdateWall();
								ParentWall->UnHighLightBorder();

							}
						}
					}
					else
					{
						Actor->AttachToActor(ParentActor, FAttachmentTransformRules::KeepWorldTransform);
					}
				}

			}
		}
	}
}


void USaveAndLoadManager::ClearWholeWorld()
{
	for (TActorIterator<AArchActor> It(GetWorld()); It; ++It)
	{
		if (AArchActor* Actor = *It; IsValid(Actor))
		{
			Actor->Destroy();
		}
	}

}

void USaveAndLoadManager::SynchronizeScrollListUI()
{
	if(IsValid(SaveAndLoadUI))
	{
	
		SaveAndLoadUI->PopulateSlotsList(GetSaveSlots());
	}
}