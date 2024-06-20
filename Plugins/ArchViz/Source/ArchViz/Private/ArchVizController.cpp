// Fill out your copyright notice in the Description page of Project Settings.


#include  "ArchVizController.h"
#include  "EnhancedInputComponent.h"
#include  "InputAction.h"
#include  "InputMappingContext.h"
#include  "EnhancedInputSubsystems.h"
#include "Components/Button.h"
#include  "Components/SplineComponent.h"
#include "House/DoorActor.h"
#include "House/FloorActor.h"
#include "House/WallActor.h"


AArchVizController::AArchVizController()
{
    bShowMouseCursor = true;
}

void AArchVizController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

    if (IsCurrentActorMoving())
    {
        UpdateActorPlacement();
    }
}

void AArchVizController::CreateAndSelectWall()
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

    bIsMovingWithCursor = true;
}

void AArchVizController::OnWallLengthChange(float InValue)
{
    if(IsValid(SelectedActor))
    {
	    if(auto Wall = Cast<AWallActor>(SelectedActor))
	    {
            Wall->SetLength(InValue);
	    }
    }
}

void AArchVizController::DeleteSelectedActor()
{
    AActor* Temp = SelectedActor;
    DeselectCurrentActor();
    Temp->Destroy();
    
}

void AArchVizController::CreateAndSelectFloor(){

    if (IsCurrentActorMoving())
    {
        bIsMovingWithCursor = false;
        SelectedActor->Destroy();
        SelectedActor = nullptr;
    }

    DeselectCurrentActor();


    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    SelectedActor = GetWorld()->SpawnActor<AHouseComponent>(FloorClass, FVector{ 0, 0, 0 },
        FRotator{ 0, 0, 0 },
        SpawnParams);

    SelectedActor->HighLightBorder();

    bIsMovingWithCursor = true;
}

void AArchVizController::CreateAndSelectDoor()
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


void AArchVizController::BeginPlay()
{
	Super::BeginPlay();

    MainUI = CreateWidget<UMainControllerUI>(this,MainUIClass);

    if(IsValid(MainUI))
    {
        MainUI->RoadButton->OnClicked.AddDynamic(this, &AArchVizController::InitRoadConstructionMode);
        MainUI->HouseButton->OnClicked.AddDynamic(this, &AArchVizController::InitHouseConstructionMode);
        MainUI->MaterialButton->OnClicked.AddDynamic(this, &AArchVizController::InitMaterialChangeMode);
        MainUI->InteriorButton->OnClicked.AddDynamic(this, &AArchVizController::InitInteriorDesignMode);

        MainUI->AddToViewport(1);
    }

    HouseConstructionUI = CreateWidget<UHouseConstructionWidget>(this, HouseConstructionUIClass);

    if(IsValid(HouseConstructionUI))
    {
        HouseConstructionUI->WallButton->OnClicked.AddDynamic(this, &AArchVizController::CreateAndSelectWall);
        HouseConstructionUI->FloorButton->OnClicked.AddDynamic(this, &AArchVizController::CreateAndSelectFloor);
        HouseConstructionUI->DoorButton->OnClicked.AddDynamic(this, &AArchVizController::CreateAndSelectDoor);
    }


}


void AArchVizController::CleanUp()
{

    if(IsCurrentActorMoving())
    {
        DestroySelectedActor();
    }

    HouseConstructionUI->RemoveFromParent();
}

void AArchVizController::InitRoadConstructionMode()
{
    CleanUp();

    CurrentMode = EMode::RoadConstruction;

    SetUpInputForRoadConstructionMode();
}

void AArchVizController::InitHouseConstructionMode()
{
    CleanUp();

    CurrentMode = EMode::HouseConstruction;

    SetUpInputForHouseConstructionMode();

    HouseConstructionUI->AddToViewport();
}

void AArchVizController::InitInteriorDesignMode()
{

    CleanUp();

    CurrentMode = EMode::InteriorDesign;

    SetUpInputForInteriorDesignMode();
}

void AArchVizController::InitMaterialChangeMode()
{

    CleanUp();

    CurrentMode = EMode::MaterialChanger;
}





void AArchVizController::OnLeftMouseButtonDown(const FInputActionValue& InputActionValue)
{
     
    if (CurrentMode == EMode::RoadConstruction)
    {
        FVector WorldLocation, WorldDirection;
        DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

        FHitResult HitResult;
        GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * 100000.0f, ECC_Visibility);

        if (HitResult.bBlockingHit)
        {
            FVector HitLocation = HitResult.Location;

            if (CurrentRoadSpline == nullptr)
            {
                CurrentRoadSpline = GetWorld()->SpawnActor<ARoadSplineActor>(RoadSplineClass, HitLocation, FRotator::ZeroRotator);
                CurrentRoadSpline->SplinePoints.Add(HitLocation);
                CurrentRoadSpline->UpdateRoad();

            }
            else
            {
                CurrentRoadSpline->SplinePoints.Add(HitLocation);
                CurrentRoadSpline->UpdateRoad();
            }
        }
    }
    else if (CurrentMode == EMode::HouseConstruction)
    {
        if (IsValid(SelectedActor))
        {
            if (IsCurrentActorMoving())
            {
                EndActorPlacement();

                if(IsValid(SelectedActor->PropertyPanelUI))
                {
                    SelectedActor->ShowPropertyPanel();
                }
                
            }
            else
            {
                if (SelectedActor != GetActorUnderCursor())
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
    }
	
}

void AArchVizController::SetupInputComponent()
{
	Super::SetupInputComponent();

}



void AArchVizController::OnRKeyDown(const FInputActionValue& InputActionValue)
{
    if(IsValid(SelectedActor))
    {
        SelectedActor->Rotate90Degree();
    }
}



bool AArchVizController::IsCurrentActorMoving()
{
    return bIsMovingWithCursor;
};

void AArchVizController::SelectHouseComponentActorUnderCursor()
{
    AActor* ActorUnderCursor = GetActorUnderCursor();


    if(IsValid(ActorUnderCursor))
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

AActor* AArchVizController::GetActorUnderCursor()
{
    if (FVector StartLocation, WorldDirection; DeprojectMousePositionToWorld(StartLocation, WorldDirection))
    {
        FVector EndLocation = StartLocation + WorldDirection * 100000;

        FCollisionQueryParams Params;
        Params.bTraceComplex = true;
        Params.AddIgnoredActor(GetPawn());

        if (FHitResult HitResult; GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation,
            ECC_Visibility, Params))
        {
            return HitResult.GetActor();
        }
    }

    return nullptr;
};

void AArchVizController::DeselectCurrentActor()
{
    if(IsValid(SelectedActor))
    {
		 SelectedActor->UnHighLightBorder();

        if(IsValid(SelectedActor->PropertyPanelUI))
        {
			SelectedActor->HidePropertyPanel();
        }
    }

	SelectedActor = nullptr;


};


USceneComponent* AArchVizController::GetComponentUnderCursor()
{
    if (FVector StartLocation, WorldDirection; DeprojectMousePositionToWorld(StartLocation, WorldDirection))
    {
        FVector EndLocation = StartLocation + WorldDirection * 100000;

        FCollisionQueryParams Params;
        Params.bTraceComplex = true;
        Params.AddIgnoredActor(GetPawn());
        Params.AddIgnoredActor(SelectedActor);

        if (FHitResult HitResult; GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation,
            ECC_Visibility, Params))
        {
            return HitResult.GetComponent();
        }
    }

    return nullptr;
}

void AArchVizController::StartActorPlacement()
{
    if(IsValid(SelectedActor))
    {
    	bIsMovingWithCursor = true;
    }
};

void AArchVizController::EndActorPlacement()
{
    bIsMovingWithCursor = false;

    if(SelectedActor->IsA(ADoorActor::StaticClass()))
    {
        auto HitComponent = GetComponentUnderCursor();
        auto HitActor = HitComponent->GetAttachParentActor();
        
        if(auto Wall = Cast<AWallActor>(HitActor))
        {
            Wall->AttachDoorToComponent(Cast<UStaticMeshComponent>(HitComponent), Cast<ADoorActor>(SelectedActor));
            Wall->UnHighLightBorder();
        }
    }
};

void AArchVizController::UpdateActorPlacement()
{
        if (SelectedActor)
        {
            if (FVector StartLocation, WorldDirection; DeprojectMousePositionToWorld(StartLocation, WorldDirection))
            {
                FVector EndLocation = StartLocation + WorldDirection * 100000;

                FCollisionQueryParams Params;
                Params.bTraceComplex = true;
                Params.AddIgnoredActor(GetPawn());
                Params.AddIgnoredActor(SelectedActor);


                if (FHitResult HitResult; GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation,
                    ECC_Visibility, Params))
                {
                   
                        FVector NewLocation = HitResult.Location;
                        SelectedActor->SetActorLocation(NewLocation);
                	    SelectedActor->SnapActorToGrid({ 20,20,20 });

                    
                   

                }
            }
        }
}

void AArchVizController::OnMKeyDown()
{
    StartActorPlacement();
};


void AArchVizController::SetUpInputForRoadConstructionMode()
{
    auto LeftClickAction = NewObject<UInputAction>(this);

    RoadConstructionMapping = NewObject<UInputMappingContext>(this);

    RoadConstructionMapping->MapKey(LeftClickAction, EKeys::LeftMouseButton);

    if (auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AArchVizController::OnLeftMouseButtonDown);

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
        EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Completed, this, &AArchVizController::OnRKeyDown);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AArchVizController::OnMKeyDown);
        EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AArchVizController::OnLeftMouseButtonDown);

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
	
}

void AArchVizController::DestroySelectedActor()
{
	bIsMovingWithCursor = false;
	SelectedActor->Destroy();
	SelectedActor = nullptr;

}