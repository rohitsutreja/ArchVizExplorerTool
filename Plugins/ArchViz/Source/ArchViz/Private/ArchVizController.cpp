
#include  "ArchVizController.h"
#include "EngineUtils.h"
#include  "EnhancedInputComponent.h"
#include  "EnhancedInputSubsystems.h"
#include  "InputAction.h"
#include  "InputMappingContext.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "House/FloorActor.h"
#include "House/WallActor.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/HouseConstructionManager.h"
#include "Managers/InteriorDesignManager.h"
#include "Managers/RoadConstructionManager.h"
#include "SaveAndLoad/ArchVizSave.h"
#include "Widgets/HouseConstructionWidget.h"
#include "Widgets/MainControllerUI.h"
#include "Widgets/ScrollableListWidget.h"


AArchVizController::AArchVizController() : MainUI(nullptr), HouseConstructionUI(nullptr),
                                           RoadConstructionManager(nullptr),
                                           HouseConstructionManager(nullptr),
                                           CurrentManager(nullptr),
                                           RoadConstructionMapping(nullptr),
                                           HouseConstructionMapping(nullptr),
                                           InteriorDesignMapping(nullptr)
{
	bShowMouseCursor = true;
}

void AArchVizController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HouseConstructionManager->IsCurrentActorMoving())
	{
		HouseConstructionManager->UpdateActorPlacement();
	}

	if(InteriorDesignManager->IsCurrentActorMoving())
	{
		InteriorDesignManager->UpdateActorPlacement();
	}
}


void AArchVizController::BeginPlay()
{
	Super::BeginPlay();

	MainUI = CreateWidget<UMainControllerUI>(this, MainUIClass);

	if (IsValid(MainUI))
	{
		MainUI->RoadButton->OnClicked.AddDynamic(this, &AArchVizController::InitRoadConstructionMode);
		MainUI->HouseButton->OnClicked.AddDynamic(this, &AArchVizController::InitHouseConstructionMode);
		MainUI->InteriorButton->OnClicked.AddDynamic(this, &AArchVizController::InitInteriorDesignMode);
		MainUI->SaveButton->OnClicked.AddDynamic(this, &AArchVizController::SaveGame);

		MainUI->AddToViewport(1);
	}

	RoadConstructionManager = NewObject<URoadConstructionManager>(this, RoadConstructionManagerClass);
	HouseConstructionManager = NewObject<UHouseConstructionManager>(this, HouseConstructionManagerClass);
	InteriorDesignManager = NewObject<UInteriorDesignManager>(this, InteriorDesignManagerClass);

	if (IsValid(RoadConstructionManager))
	{
		RoadConstructionManager->SetUp();
	}

	if (IsValid(HouseConstructionManager))
	{
		HouseConstructionManager->SetUp();
	}

	if(IsValid(InteriorDesignManager))
	{
		InteriorDesignManager->SetUp();
	}


	LoadGame();
}

void AArchVizController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(IsValid(CurrentManager))
	{
		CurrentManager->End();
	}

	Super::EndPlay(EndPlayReason);
}

FSlateBrush AArchVizController::GetBrushWithTint(const FLinearColor& Color)
{
	FSlateBrush Brush = FSlateBrush();
	Brush.DrawAs = ESlateBrushDrawType::RoundedBox;
	Brush.OutlineSettings.RoundingType = ESlateBrushRoundingType::Type::FixedRadius;
	Brush.OutlineSettings.CornerRadii = FVector4d(10,10,10,10);
	Brush.TintColor = FLinearColor(Color);

	return Brush;
};

void AArchVizController::CleanUp()
{
	if (IsValid(CurrentManager))
	{
		CurrentManager->End();
	}

	MainUI->RoadButtonBorder->SetBrush(GetBrushWithTint(FColor::Black));
	MainUI->HouseButtonBorder->SetBrush(GetBrushWithTint(FColor::Black));
	MainUI->InteriorButtonBorder->SetBrush(GetBrushWithTint(FColor::Black));


}

void AArchVizController::InitRoadConstructionMode()
{
	CleanUp();

	CurrentMode = EMode::RoadConstruction;

	MainUI->RoadButtonBorder->SetBrush(GetBrushWithTint(FColor::Green));

	SetUpInputForRoadConstructionMode();

	CurrentManager = RoadConstructionManager;

	if (IsValid(CurrentManager))
	{
		CurrentManager->Start();
	}
}

void AArchVizController::InitHouseConstructionMode()
{
	CleanUp();

	CurrentMode = EMode::HouseConstruction;

	MainUI->HouseButtonBorder->SetBrush(GetBrushWithTint(FColor::Green));

	SetUpInputForHouseConstructionMode();

	CurrentManager = HouseConstructionManager;

	if (IsValid(CurrentManager))
	{
		CurrentManager->Start();
	}
}

void AArchVizController::InitInteriorDesignMode()
{

	CleanUp();

	CurrentMode = EMode::InteriorDesign;

	MainUI->InteriorButtonBorder->SetBrush(GetBrushWithTint(FColor::Green));

	SetUpInputForInteriorDesignMode();

	CurrentManager = InteriorDesignManager;

	if (IsValid(CurrentManager))
	{
		CurrentManager->Start();
	}

}


AActor* AArchVizController::GetActorUnderCursor(const TArray<AActor*>& IgnoredActors)
{
	const FHitResult HitResult = GetHitResult(IgnoredActors);

	if (AActor* HitActor = HitResult.GetActor())
	{
		return HitActor;
	}

	return nullptr;
};

UPrimitiveComponent* AArchVizController::GetComponentUnderCursor(const TArray<AActor*>& IgnoredActors)
{
	const FHitResult HitResult = GetHitResult(IgnoredActors);

	if (UPrimitiveComponent* HitComponent = HitResult.GetComponent())
	{
		return HitComponent;
	}

	return nullptr;
}

FHitResult AArchVizController::GetHitResult(const TArray<AActor*>& IgnoredActors) const
{
	FHitResult HitResult;

	if (FVector StartLocation, WorldDirection; DeprojectMousePositionToWorld(StartLocation, WorldDirection))
	{
		FVector EndLocation = StartLocation + WorldDirection * 100000;

		FCollisionQueryParams Params;
		Params.bTraceComplex = true;

		Params.AddIgnoredActor(GetPawn());
		Params.AddIgnoredActors(IgnoredActors);


		GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation,
			ECC_Visibility, Params);
	}
	return HitResult;
}




void AArchVizController::SetUpInputForRoadConstructionMode()
{
	auto LeftClickAction = NewObject<UInputAction>(this);
	auto UKeyAction = NewObject<UInputAction>(this);

	RoadConstructionMapping = NewObject<UInputMappingContext>(this);

	RoadConstructionMapping->MapKey(LeftClickAction, EKeys::LeftMouseButton);
	RoadConstructionMapping->MapKey(UKeyAction, EKeys::Z);


	if (auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, RoadConstructionManager, &URoadConstructionManager::OnLeftClick);
		EnhancedInputComponent->BindAction(UKeyAction, ETriggerEvent::Completed, RoadConstructionManager, &URoadConstructionManager::OnUKeyDown);


		if (auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			if (IsValid(HouseConstructionMapping))
			{
				InputSubsystem->RemoveMappingContext(HouseConstructionMapping);
			}
			if (IsValid(InteriorDesignMapping))
			{
				InputSubsystem->RemoveMappingContext(InteriorDesignMapping);
			}
			if (IsValid(RoadConstructionMapping))
			{
				InputSubsystem->AddMappingContext(RoadConstructionMapping, 0);
			}
		};
	}
}

void AArchVizController::SetUpInputForHouseConstructionMode()
{
	auto RotateAction = NewObject<UInputAction>(this);
	auto MoveAction = NewObject<UInputAction>(this);
	auto LeftClickAction = NewObject<UInputAction>(this);

	HouseConstructionMapping = NewObject<UInputMappingContext>(this);
	HouseConstructionMapping->MapKey(RotateAction, EKeys::R);
	HouseConstructionMapping->MapKey(MoveAction, EKeys::M);

	HouseConstructionMapping->MapKey(LeftClickAction, EKeys::LeftMouseButton);


	if (auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Completed, HouseConstructionManager, &UHouseConstructionManager::OnRKeyDown);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, HouseConstructionManager, &UHouseConstructionManager::OnMKeyDown);
		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, HouseConstructionManager, &UHouseConstructionManager::OnLeftClick);

		if (auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			if (IsValid(RoadConstructionMapping))
			{
				InputSubsystem->RemoveMappingContext(RoadConstructionMapping);
			}
			if (IsValid(InteriorDesignMapping))
			{
				InputSubsystem->RemoveMappingContext(InteriorDesignMapping);
			}
			if (IsValid(HouseConstructionMapping))
			{
				InputSubsystem->AddMappingContext(HouseConstructionMapping, 0);
			}
		};
	}
}

void AArchVizController::SetUpInputForInteriorDesignMode()
{
	auto RotateAction = NewObject<UInputAction>(this);
	auto MoveAction = NewObject<UInputAction>(this);
	auto LeftClickAction = NewObject<UInputAction>(this);

	InteriorDesignMapping = NewObject<UInputMappingContext>(this);

	InteriorDesignMapping->MapKey(RotateAction, EKeys::R);
	InteriorDesignMapping->MapKey(MoveAction, EKeys::M);
	InteriorDesignMapping->MapKey(LeftClickAction, EKeys::LeftMouseButton);


	if (auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Completed, InteriorDesignManager, &UInteriorDesignManager::OnRKeyDown);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, InteriorDesignManager, &UInteriorDesignManager::OnMKeyDown);
		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, InteriorDesignManager, &UInteriorDesignManager::OnLeftClick);

		if (auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			if (IsValid(RoadConstructionMapping))
			{
				InputSubsystem->RemoveMappingContext(RoadConstructionMapping);
			}
			if (IsValid(HouseConstructionMapping))
			{
				InputSubsystem->RemoveMappingContext(HouseConstructionMapping);
			}
			if (IsValid(InteriorDesignMapping))
			{
				InputSubsystem->AddMappingContext(InteriorDesignMapping, 0);
			}
		};
	}
}


void AArchVizController::SaveGame()
{

	CleanUp();

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
		RoadData.ParentActorId = RoadActor->GetAttachParentActor() ? Cast<AArchActor>(RoadActor->GetAttachParentActor())->GetId() : -1;
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

		
		WallData.ParentActorId = WallActor->GetAttachParentActor() ? Cast<AArchActor>(WallActor->GetAttachParentActor())->GetId() : -1;

		SaveGameInstance->WallActorArray.Add(WallData);
	}

	for (TActorIterator<AFloorActor> It(GetWorld()); It; ++It)
	{
		AFloorActor* FloorActor = *It;
		FFloor FloorData;
		FloorData.ID = FloorActor->GetId();

		FloorData.Transform = FloorActor->GetActorTransform();
		FloorData.Material = FloorActor->GetMaterial();
		FloorData.Dimensions = FloorActor->GetDimensions();
		FloorData.ParentActorId = FloorActor->GetAttachParentActor() ? Cast<AArchActor>(FloorActor->GetAttachParentActor())->GetId() : -1;
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

		DoorData.ParentActorId = DoorActor->GetAttachParentActor() ? Cast<AArchActor>(DoorActor->GetAttachParentActor())->GetId() : -1;
		SaveGameInstance->DoorActorArray.Add(DoorData);
	}


	for (TActorIterator<AInteriorActor> It(GetWorld()); It; ++It)
	{
		AInteriorActor* InteriorActor = *It;
		FInterior InteriorData;
		InteriorData.ID = InteriorActor->GetId();
		InteriorData.Transform = InteriorActor->GetActorTransform();
		InteriorData.StaticMesh = InteriorActor->GetCurrentStaticMesh();
		InteriorData.ParentActorId = InteriorActor->GetAttachParentActor() ? Cast<AArchActor>(InteriorActor->GetAttachParentActor())->GetId() : -1;
		SaveGameInstance->InteriorActorArray.Add(InteriorData);
	}


	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Rohit1"), 0);
}

void AArchVizController::LoadGame()
{
	UArchVizSave* LoadGameInstance = Cast<UArchVizSave>(UGameplayStatics::LoadGameFromSlot(TEXT("Rohit1"), 0));
	

	if (LoadGameInstance)
	{
		TMap<int32, AActor*> IdToActorMap;
		TMap<AActor*, int32> ActorToParentActorIdMap;

		for (const FRoad& RoadData : LoadGameInstance->RoadActorArray)
		{
			ARoadSplineActor* RoadActor = GetWorld()->SpawnActor<ARoadSplineActor>(RoadConstructionManager->RoadSplineClass, RoadData.Transform);
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
			AWallActor* WallActor = GetWorld()->SpawnActor<AWallActor>(HouseConstructionManager->WallClass, WallData.Transform);
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
			AFloorActor* FloorActor = GetWorld()->SpawnActor<AFloorActor>(HouseConstructionManager->FloorClass, FloorData.Transform);
			FloorActor->SetActorTransform(FloorData.Transform);
			FloorActor->SetDimensions(FloorData.Dimensions);
			FloorActor->SetMaterial(FloorData.Material);
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
			ADoorActor* DoorActor = GetWorld()->SpawnActor<ADoorActor>(HouseConstructionManager->DoorClass, DoorData.Transform);
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
			AInteriorActor* InteriorActor = GetWorld()->SpawnActor<AInteriorActor>(InteriorDesignManager->InteriorActorClass, InteriorData.Transform);
			InteriorActor->SetActorTransform(InteriorData.Transform);
			InteriorActor->SetStaticMesh(InteriorData.StaticMesh);
			IdToActorMap.Add(InteriorData.ID, InteriorActor);

			if (InteriorData.ParentActorId != -1)
			{
				ActorToParentActorIdMap.Add(InteriorActor, InteriorData.ParentActorId);
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
						if (auto ParentWall = Cast<AWallActor>(ParentActor) ; IsValid(ParentWall))
						{
							auto Door = Cast<ADoorActor>(Actor);
							ParentWall->AddDoorAtIndex(Door->ParentWallComponentIndex, Door);
							ParentWall->UpdateWall();
							ParentWall->UnHighLightBorder();
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
