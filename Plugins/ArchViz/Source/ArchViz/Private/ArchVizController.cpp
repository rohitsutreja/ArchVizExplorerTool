#include "ArchVizController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Managers/HouseConstructionManager.h"
#include "Managers/HouseTemplateManager.h"
#include "Managers/InteriorDesignManager.h"
#include "Managers/RoadConstructionManager.h"
#include "Managers/SaveAndLoadManager.h"
#include "Widgets/HouseConstructionWidget.h"
#include "Widgets/MainControllerUI.h"


AArchVizController::AArchVizController()
    : MainUI(nullptr), HouseConstructionUI(nullptr),
    RoadConstructionManager(nullptr), HouseConstructionManager(nullptr),
    InteriorDesignManager(nullptr), SaveAndLoadManager(nullptr),
    CurrentManager(nullptr), RoadConstructionMapping(nullptr),
    HouseConstructionMapping(nullptr), InteriorDesignMapping(nullptr)
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

    if (InteriorDesignManager->IsCurrentActorMoving())
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
        MainUI->SaveButton->OnClicked.AddDynamic(this, &AArchVizController::InitSaveMode);
        MainUI->MenuButton->OnClicked.AddDynamic(this, &AArchVizController::InitLoadMode);
        MainUI->TemplateButton->OnClicked.AddDynamic(this, &AArchVizController::InitHouseTemplateMode);

        MainUI->AddToViewport(1);
    }

    InitializeAndSetUpManagers();

	InitLoadMode();
}

void AArchVizController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (IsValid(CurrentManager))
    {
        CurrentManager->End();
    }

    Super::EndPlay(EndPlayReason);
}


void AArchVizController::InitializeAndSetUpManagers()
{
	RoadConstructionManager = NewObject<URoadConstructionManager>(this, RoadConstructionManagerClass);
	HouseConstructionManager = NewObject<UHouseConstructionManager>(this, HouseConstructionManagerClass);
	InteriorDesignManager = NewObject<UInteriorDesignManager>(this, InteriorDesignManagerClass);
    HouseTemplateManager = NewObject<UHouseTemplateManager>(this , HouseTemplateManagerClass);
	SaveAndLoadManager = NewObject<USaveAndLoadManager>(this, SaveAndLoadManagerClass);

    if (IsValid(RoadConstructionManager))
    {
        RoadConstructionManager->SetUp();
    }

    if (IsValid(HouseConstructionManager))
    {
        HouseConstructionManager->SetUp();
    }

    if (IsValid(InteriorDesignManager))
    {
        InteriorDesignManager->SetUp();
    }

    if (IsValid(SaveAndLoadManager))
    {
        SaveAndLoadManager->SetUp();
    }

    if(IsValid(HouseTemplateManager))
    {
        HouseTemplateManager->SetUp();
    }
}


FSlateBrush AArchVizController::GetBrushWithTint(const FLinearColor& Color)
{
    FSlateBrush Brush = FSlateBrush();
    Brush.DrawAs = ESlateBrushDrawType::RoundedBox;
    Brush.OutlineSettings.RoundingType = ESlateBrushRoundingType::Type::FixedRadius;
    Brush.OutlineSettings.CornerRadii = FVector4d(10, 10, 10, 10);
    Brush.TintColor = FLinearColor(Color);

    return Brush;
}

AHouseTemplate* AArchVizController::GetSavedHouseTemplate(const FString& TemplateName)
{
    if(IsValid(SaveAndLoadManager))
    {
		return SaveAndLoadManager->GetHouseTemplate(TemplateName);
    }
    return nullptr;
}

void AArchVizController::CleanUp() const
{
    if (IsValid(CurrentManager))
    {
        CurrentManager->End();
    }

    ClearAllInputMappings();

    switch (CurrentMode)
    {
    case EMode::RoadConstruction:
        MainUI->RoadButtonBorder->SetBrush(GetBrushWithTint(FColor::Black));
        break;
    case EMode::HouseConstruction:
        MainUI->HouseButtonBorder->SetBrush(GetBrushWithTint(FColor::Black));
        break;
    case EMode::InteriorDesign:
        MainUI->InteriorButtonBorder->SetBrush(GetBrushWithTint(FColor::Black));
        break;
    case EMode::HouseTemplate:
        MainUI->TemplateButtonBorder->SetBrush(GetBrushWithTint(FColor::Black));
        break;
    case EMode::SaveMode:
        MainUI->SaveButtonBorder->SetBrush(GetBrushWithTint(FColor::Black));
        break;
    case EMode::LoadMode:
        MainUI->MenuButtonBorder->SetBrush(GetBrushWithTint(FColor::Black));
        break;
    }
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

void AArchVizController::InitHouseTemplateMode()
{
    CleanUp();
    CurrentMode = EMode::HouseTemplate;
    MainUI->TemplateButtonBorder->SetBrush(GetBrushWithTint(FColor::Green));
    SetUpInputForHouseTemplateMode();
    CurrentManager = HouseTemplateManager;
    if(IsValid(HouseTemplateManager))
    {
		 HouseTemplateManager->Start();
    }
    
}

void AArchVizController::InitSaveMode()
{
    CleanUp();
    CurrentMode = EMode::SaveMode;
    MainUI->SaveButtonBorder->SetBrush(GetBrushWithTint(FColor::Green));
    CurrentManager = SaveAndLoadManager;

    if (IsValid(CurrentManager))
    {
        CurrentManager->Start();
    }
}

void AArchVizController::InitLoadMode()
{
    CleanUp();
    CurrentMode = EMode::LoadMode;
    MainUI->MenuButtonBorder->SetBrush(GetBrushWithTint(FColor::Green));
    CurrentManager = SaveAndLoadManager;

    if (IsValid(CurrentManager))
    {
        CurrentManager->Start();
    }
}

AActor* AArchVizController::GetActorUnderCursor(const TArray<AActor*>& IgnoredActors) const
{
    const FHitResult HitResult = GetHitResult(IgnoredActors);
    return HitResult.GetActor();
}

UPrimitiveComponent* AArchVizController::GetComponentUnderCursor(const TArray<AActor*>& IgnoredActors) const
{
    const FHitResult HitResult = GetHitResult(IgnoredActors);
    return HitResult.GetComponent();
}

FHitResult AArchVizController::GetHitResult(const TArray<AActor*>& IgnoredActors) const
{
    FHitResult HitResult;
    FVector StartLocation, WorldDirection;

    if (DeprojectMousePositionToWorld(StartLocation, WorldDirection))
    {
        FVector EndLocation = StartLocation + WorldDirection * 100000;
        FCollisionQueryParams Params;
        Params.bTraceComplex = true;
        Params.AddIgnoredActor(GetPawn());
        Params.AddIgnoredActors(IgnoredActors);

        GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
    }
    return HitResult;
}

EMode AArchVizController::GetCurrentMode() const
{
    return  CurrentMode;
}

UMainControllerUI* AArchVizController::GetMainUI() const
{
    return MainUI;
}

void AArchVizController::ClearAllInputMappings() const
{
    if (Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (const auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
        {
            InputSubsystem->ClearAllMappings();
        }
    }
}

void AArchVizController::SetUpInputForRoadConstructionMode()
{
    const auto LeftClickAction = NewObject<UInputAction>(this);
    const auto UKeyAction = NewObject<UInputAction>(this);
    RoadConstructionMapping = NewObject<UInputMappingContext>(this);
    RoadConstructionMapping->MapKey(LeftClickAction, EKeys::LeftMouseButton);
    RoadConstructionMapping->MapKey(UKeyAction, EKeys::Z);

    if (const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, RoadConstructionManager, &URoadConstructionManager::OnLeftClick);
        EnhancedInputComponent->BindAction(UKeyAction, ETriggerEvent::Completed, RoadConstructionManager, &URoadConstructionManager::OnUKeyDown);

        if (const auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
        {
            InputSubsystem->ClearAllMappings();
            if (IsValid(RoadConstructionMapping))
            {
                InputSubsystem->AddMappingContext(RoadConstructionMapping, 0);
            }
        }
    }
}

void AArchVizController::SetUpInputForHouseConstructionMode()
{
    const auto RotateAction = NewObject<UInputAction>(this);
    const auto MoveAction = NewObject<UInputAction>(this);
    const auto LeftClickAction = NewObject<UInputAction>(this);
    HouseConstructionMapping = NewObject<UInputMappingContext>(this);
    HouseConstructionMapping->MapKey(RotateAction, EKeys::R);
    HouseConstructionMapping->MapKey(MoveAction, EKeys::M);
    HouseConstructionMapping->MapKey(LeftClickAction, EKeys::LeftMouseButton);

    if (const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Completed, HouseConstructionManager, &UHouseConstructionManager::OnRKeyDown);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, HouseConstructionManager, &UHouseConstructionManager::OnMKeyDown);
        EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, HouseConstructionManager, &UHouseConstructionManager::OnLeftClick);

        if (const auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
        {
            InputSubsystem->ClearAllMappings();
            if (IsValid(HouseConstructionMapping))
            {
                InputSubsystem->AddMappingContext(HouseConstructionMapping, 0);
            }
        }
    }
}

void AArchVizController::SetUpInputForInteriorDesignMode()
{
    const auto RotateAction = NewObject<UInputAction>(this);
    const auto MoveAction = NewObject<UInputAction>(this);
    const auto LeftClickAction = NewObject<UInputAction>(this);
    InteriorDesignMapping = NewObject<UInputMappingContext>(this);
    InteriorDesignMapping->MapKey(RotateAction, EKeys::R);
    InteriorDesignMapping->MapKey(MoveAction, EKeys::M);
    InteriorDesignMapping->MapKey(LeftClickAction, EKeys::LeftMouseButton);

    if (const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Completed, InteriorDesignManager, &UInteriorDesignManager::OnRKeyDown);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, InteriorDesignManager, &UInteriorDesignManager::OnMKeyDown);
        EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, InteriorDesignManager, &UInteriorDesignManager::OnLeftClick);

        if (const auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
        {
            InputSubsystem->ClearAllMappings();
            if (IsValid(InteriorDesignMapping))
            {
                InputSubsystem->AddMappingContext(InteriorDesignMapping, 0);
            }
        }
    }
}

void AArchVizController::SetUpInputForHouseTemplateMode()
{
    const auto RotateAction = NewObject<UInputAction>(this);
    const auto LeftClickAction = NewObject<UInputAction>(this);

    HouseTemplateMapping = NewObject<UInputMappingContext>(this);
    HouseTemplateMapping->MapKey(RotateAction, EKeys::R);
    HouseTemplateMapping->MapKey(LeftClickAction, EKeys::LeftMouseButton);

    if (const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Completed, HouseTemplateManager, &UHouseTemplateManager::OnRKeyDown);
        EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, HouseTemplateManager, &UHouseTemplateManager::OnLeftClick);

        if (const auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
        {
            InputSubsystem->ClearAllMappings();
            if (IsValid(HouseTemplateManager))
            {
                InputSubsystem->AddMappingContext(HouseTemplateMapping, 0);
            }
        }
    }
}

