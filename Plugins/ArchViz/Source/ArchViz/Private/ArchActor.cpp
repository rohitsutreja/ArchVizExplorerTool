#include "ArchActor.h"

#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "House/HouseComponent.h"


AArchActor::AArchActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AArchActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArchActor::BeginPlay()
{
	Super::BeginPlay();

	ID = GenerateID();

	PropertyPanelUI = CreateWidget<UPropertyPanelWidget>(GetWorld(), PropertyPanelClass);

	if (IsValid(PropertyPanelUI))
	{
		PropertyPanelUI->DeleteWallButton->OnClicked.AddDynamic(this, &AHouseComponent::OnDeleteButtonClicked);
	}

}

void AArchActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(PropertyPanelUI))
	{
		PropertyPanelUI->RemoveFromParent();
	}

	Super::EndPlay(EndPlayReason);
}

void AArchActor::ShowPropertyPanel()
{
	if (IsValid(PropertyPanelUI))
	{
		PropertyPanelUI->AddToViewport();
	}
}

void AArchActor::HidePropertyPanel()
{
	if (IsValid(PropertyPanelUI))
	{
		PropertyPanelUI->RemoveFromParent();
	}
}

int32 AArchActor::GetId()
{
	return ID;
}

void AArchActor::OnDeleteButtonClicked()
{
	Destroy();
}

