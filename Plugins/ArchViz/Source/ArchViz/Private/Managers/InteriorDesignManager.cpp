// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/InteriorDesignManager.h"

#include "ArchVizController.h"
#include "Blueprint/UserWidget.h"
#include "House/FloorActor.h"
#include "House/WallActor.h"
#include "Interior/InteriorActor.h"
#include "Widgets/InteriorDesignWidget.h"
#include "Widgets/PropertyPanelWidget.h"

void UInteriorDesignManager::SetUp()
{
	Super::SetUp();

	InteriorDesignUI = CreateWidget<UInteriorDesignWidget>(GetWorld(), InteriorDesignUIClass);

	if(IsValid(InteriorDesignUI))
	{
		InteriorDesignUI->MeshItemsScrollList->OnItemChange.AddDynamic(this, &UInteriorDesignManager::OnMeshItemClicked);
	}
	
}


bool UInteriorDesignManager::IsPlacementValid(AInteriorActor* Actor)
{
	auto HitResult = Controller->GetHitResult({ Actor });

	if(HitResult.IsValidBlockingHit())
	{
		if (auto HitActor = HitResult.GetActor(); IsValid(HitActor))
		{
			switch (Actor->GetCategory())
			{
			case EItemCategory::FloorPlaceable:
			{
				if (HitActor->IsA(AFloorActor::StaticClass()))
				{
					if (HitResult.ImpactNormal.Z > 0)
					{
						return true;
					}
				}
				Controller->Notify(TEXT("This item can be placed on floor only."));

				break;
			}
			case EItemCategory::WallPlaceable:
			{
				if (HitActor->IsA(AWallActor::StaticClass()))
				{
					return true;
				}
				Controller->Notify(TEXT("This item can be placed on wall only."));
				break;
			}
			case EItemCategory::CeilingPlaceable:
			{
				if (HitActor->IsA(AFloorActor::StaticClass()))
				{
					if (HitResult.ImpactNormal.Z < 0)
					{
						return true;
					}
				}
				Controller->Notify(TEXT("This item can be placed on ceiling only."));
				break;
			}
			case EItemCategory::TablePlaceable:
			{
				if (auto Table = Cast<AInteriorActor>(HitActor))
				{
					if (Table->GetCategory() == EItemCategory::Table)
					{
						return true;
					}
				}
				Controller->Notify(TEXT("This item can be placed on Table only."));
				break;
			}
			case EItemCategory::Table:
			{
				if (HitActor->IsA(AFloorActor::StaticClass()))
				{
					if (HitResult.ImpactNormal.Z > 0)
					{
						return true;
					}
				}
				Controller->Notify(TEXT("This item can be placed on floor only."));

				break;
			}
			}
		}
	}
	
	return false;
}

void UInteriorDesignManager::End()
{
	if (IsCurrentActorMoving())
	{
		DestroySelectedActor();
	}

	DeselectCurrentActor();

	if (IsValid(InteriorDesignUI))
	{
		InteriorDesignUI->RemoveFromParent();
	}
}

void UInteriorDesignManager::Start()
{
	Controller->Notify(TEXT("Interior Design Mode Started."));

	if (IsValid(InteriorDesignUI))
	{
		InteriorDesignUI->AddToViewport(0);
	}
}

void UInteriorDesignManager::CreateAndSelectActor(UStaticMesh* StaticMesh)
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

	SelectedActor = GetWorld()->SpawnActor<AInteriorActor>(InteriorActorClass, FVector{0, 0, 0},
		FRotator{ 0, 0, 0 },
		SpawnParams);

	SelectedActor->SetStaticMesh(StaticMesh);


	SelectedActor->HighLightBorder();

	bIsMovingWithCursor = true;
}

void UInteriorDesignManager::OnMeshItemClicked(FItemInfo ItemInfo)
{
	CreateAndSelectActor(ItemInfo.StaticMesh);
	if(IsValid(SelectedActor))
	{
	SelectedActor->SetCategory(ItemInfo.Category);
	}
}

void UInteriorDesignManager::SelectInteriorActorUnderCursor()
{
	AActor* ActorUnderCursor = Controller->GetActorUnderCursor({});


	if (IsValid(ActorUnderCursor))
	{
		if (AInteriorActor* InteriorActor = Cast<AInteriorActor>(ActorUnderCursor))
		{
			SelectedActor = InteriorActor;

			SelectedActor->HighLightBorder();

			if (IsValid(SelectedActor->PropertyPanelUI))
			{
				SelectedActor->ShowPropertyPanel();
			}
		}
	}
}

void UInteriorDesignManager::DeselectCurrentActor()
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
}

void UInteriorDesignManager::StartActorPlacement()
{
	if (IsValid(SelectedActor))
	{

		SelectedActor->HidePropertyPanel();
		bIsMovingWithCursor = true;
	}
}

void UInteriorDesignManager::EndActorPlacement()
{
	if(IsValid(SelectedActor))
	{
		if (IsPlacementValid(SelectedActor))
		{
			auto HitActor = Controller->GetActorUnderCursor({ SelectedActor });

			if(IsValid(HitActor))
			{
				
				SelectedActor->AttachToComponent(HitActor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			}

			bIsMovingWithCursor = false;
		}
	}
	

}

void UInteriorDesignManager::UpdateActorPlacement()
{
	if (IsValid(SelectedActor))
	{
		const FHitResult HitResult = Controller->GetHitResult({ SelectedActor });

		SelectedActor->SetActorLocation(HitResult.Location);
		SelectedActor->SnapActorToGrid(FVector{ 10 });
	}
}

void UInteriorDesignManager::DestroySelectedActor()
{
	bIsMovingWithCursor = false;
	SelectedActor->Destroy();
	SelectedActor = nullptr;
}

bool UInteriorDesignManager::IsCurrentActorMoving()
{
	return bIsMovingWithCursor;
}

void UInteriorDesignManager::OnLeftClick()
{

	if (IsValid(SelectedActor))
	{
		if (IsCurrentActorMoving())
		{
			EndActorPlacement();

			if(!IsCurrentActorMoving())
			{
				if (IsValid(SelectedActor->PropertyPanelUI))
				{
					SelectedActor->ShowPropertyPanel();
				}
			}
			

		}
		else
		{
			if (SelectedActor != Controller->GetActorUnderCursor())
			{
				DeselectCurrentActor();

				SelectInteriorActorUnderCursor();
			}
		}
	}
	else
	{
		SelectInteriorActorUnderCursor();
	}
}

void UInteriorDesignManager::OnRKeyDown()
{
	if(IsValid(SelectedActor))
	{
		SelectedActor->Rotate();
	}
}

void UInteriorDesignManager::OnMKeyDown()
{
	StartActorPlacement();
}
