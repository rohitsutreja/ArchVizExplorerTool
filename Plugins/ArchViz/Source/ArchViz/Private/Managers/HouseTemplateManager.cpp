// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/HouseTemplateManager.h"

#include "ArchActor.h"
#include "Widgets/HouseTemplateWidget.h"
#include "ArchVizController.h"
#include "Components/Button.h"

void UHouseTemplateManager::CreateAndSelectThirdHouseTemplate()
{
	Controller->Notify(TEXT("Left Click at Your Desired Location to Place House."));
	SelectedHouseTemplate = Controller->GetSavedHouseTemplate(TEXT("Template3"));
	if (IsValid(HouseTemplateUI))
	{
		HouseTemplateUI->ShowPanel();
	}
}

void UHouseTemplateManager::CreateAndSelectSecondHouseTemplate()
{
	Controller->Notify(TEXT("Left Click at Your Desired Location to Place House."));
	SelectedHouseTemplate = Controller->GetSavedHouseTemplate(TEXT("Template2"));
	if (IsValid(HouseTemplateUI))
	{
		HouseTemplateUI->ShowPanel();
	}
}

void UHouseTemplateManager::CreateAndSelectFirstHouseTemplate()
{
	Controller->Notify(TEXT("Left Click at Your Desired Location to Place House."));
	SelectedHouseTemplate = Controller->GetSavedHouseTemplate(TEXT("Template"));
	if (IsValid(HouseTemplateUI))
	{
		HouseTemplateUI->ShowPanel();
	}	
}

void UHouseTemplateManager::OnSaveButtonClicked()
{


	if(IsValid(SelectedHouseTemplate))
	{
		SelectedHouseTemplate->Destroy();
		SelectedHouseTemplate = nullptr;
	}

	Controller->Notify(TEXT("House Placement Complete."));

	if (IsValid(HouseTemplateUI))
	{
		HouseTemplateUI->HidePanel();
	}

}


void UHouseTemplateManager::SetUp()
{
	Super::SetUp();

	HouseTemplateUI = CreateWidget<UHouseTemplateWidget>(GetWorld(), HouseTemplateUIClass);

	if(IsValid(HouseTemplateUI))
	{
		HouseTemplateUI->FirstHouseButton->OnClicked.AddUniqueDynamic(this, &UHouseTemplateManager::CreateAndSelectFirstHouseTemplate);
		HouseTemplateUI->SecondHouseButton->OnClicked.AddUniqueDynamic(this, &UHouseTemplateManager::CreateAndSelectSecondHouseTemplate);
		HouseTemplateUI->ThirdHouseButton->OnClicked.AddUniqueDynamic(this, &UHouseTemplateManager::CreateAndSelectThirdHouseTemplate);

		HouseTemplateUI->SaveButton->OnClicked.AddUniqueDynamic(this, &UHouseTemplateManager::OnSaveButtonClicked);

		HouseTemplateUI->HidePanel();
	}
}

void UHouseTemplateManager::Start()
{
	Controller->Notify(TEXT("House Template Mode Started."));
	if(IsValid(HouseTemplateUI))
	{
		HouseTemplateUI->AddToViewport(0);
	}
}

void UHouseTemplateManager::End()
{
	if (IsValid(SelectedHouseTemplate))
	{
		SelectedHouseTemplate->Destroy();
	}
	SelectedHouseTemplate = nullptr;

	if (IsValid(HouseTemplateUI))
	{
		HouseTemplateUI->HidePanel();
		HouseTemplateUI->RemoveFromParent();
	}
}

void UHouseTemplateManager::OnRKeyDown()
{
	const auto OldRotation = SelectedHouseTemplate->GetActorRotation();
	auto NewRotation = OldRotation;
	NewRotation.Yaw += 90;
	SelectedHouseTemplate->SetActorRotation(NewRotation);
	
}

void UHouseTemplateManager::OnLeftClick()
{
	if(const auto HitResult = Controller->GetHitResult({ SelectedHouseTemplate }); HitResult.bBlockingHit)
	{
		if(IsValid(SelectedHouseTemplate))
		{
			if(HitResult.GetActor()->IsA(AArchActor::StaticClass())){
				Controller->Notify("Please Select Valid Location.");
				return;
			}
			SelectedHouseTemplate->SetActorLocation(HitResult.Location);
			SelectedHouseTemplate->SnapActorToGrid({ 20 ,20, 20});
		}
		else
		{
			Controller->Notify("Please Select a Template First.");
		}
	
	}
}
