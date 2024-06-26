
#include  "ArchVizController.h"
#include  "EnhancedInputComponent.h"
#include  "EnhancedInputSubsystems.h"
#include  "InputAction.h"
#include  "InputMappingContext.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Managers/HouseConstructionManager.h"
#include "Managers/InteriorDesignManager.h"
#include "Managers/RoadConstructionManager.h"
#include "Widgets/HouseConstructionWidget.h"
#include "Widgets/MainControllerUI.h"


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
		MainUI->MaterialButton->OnClicked.AddDynamic(this, &AArchVizController::InitMaterialChangeMode);

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

void AArchVizController::InitMaterialChangeMode()
{

	CleanUp();

	CurrentMode = EMode::MaterialChanger;
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

	RoadConstructionMapping = NewObject<UInputMappingContext>(this);

	RoadConstructionMapping->MapKey(LeftClickAction, EKeys::LeftMouseButton);

	if (auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, RoadConstructionManager, &URoadConstructionManager::OnLeftClick);

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
