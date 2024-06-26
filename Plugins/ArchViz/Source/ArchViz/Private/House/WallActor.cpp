// Fill out your copyright notice in the Description page of Project Settings.


#include "House/WallActor.h"

#include "ComponentUtils.h"
#include "Components/SpinBox.h"
#include "Components/TextBlock.h"
#include "House/DoorActor.h"
#include "Widgets/ScrollableListWidget.h"

AWallActor::AWallActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	NumberOfWallSegments = 5;
	LengthOfSegment = 200;
	WallSegmentMesh = nullptr;
	ArrayOfWallSegments.Empty();



}

void AWallActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);



}

void AWallActor::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(PropertyPanelUI))
	{
		PropertyPanelUI->SwitchToWidget(0);

		PropertyPanelUI->Title->SetText(FText::FromString(TEXT("Wall")));

		PropertyPanelUI->WallLengthValue->OnValueChanged.AddDynamic(this, &AWallActor::SetLength);

		PropertyPanelUI->WallMaterialList->OnMaterialChange.AddDynamic(this, &AWallActor::SetMaterial);

	}

	UpdateWall();
}

void AWallActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		RemoveAllDoorsFromWall();
	}


	Super::EndPlay(EndPlayReason);
}


void AWallActor::UpdateWall()
{

	for (auto& Temp : ArrayOfWallSegments)
	{
		if (IsValid(Temp))
		{
			Temp->DestroyComponent();
		}

	}
	ArrayOfWallSegments.Empty();


	for (int i{}; i < NumberOfWallSegments; i++)
	{
		auto WallSegment = NewObject<UStaticMeshComponent>(this);
		WallSegment->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		WallSegment->SetStaticMesh(WallSegmentMesh);
		WallSegment->SetRelativeLocation(FVector(i * LengthOfSegment, 0, 0));
		WallSegment->RegisterComponent();
		if (IsValid(Material))
		{
			WallSegment->SetMaterial(0, Material);
		}
		ArrayOfWallSegments.Add(WallSegment);
	}




	for (auto& [idx, Door] : IndexToDoorMapping)
	{

		if (idx < ArrayOfWallSegments.Num())
		{
			if (IsValid(ArrayOfWallSegments[idx]))
			{
				ArrayOfWallSegments[idx]->SetStaticMesh(DoorHallSegment);
				Door->AttachToComponent(ArrayOfWallSegments[idx], FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("DoorSocket")));
				Door->SetActorRelativeLocation(FVector::ZeroVector);
				Door->SetActorRelativeRotation(FRotator::ZeroRotator);

			}
		}

	}

	HighLightBorder();
}


void AWallActor::SetLength(float Length)
{
	auto NewNumberOfWallSegments = FMath::Floor(Length / LengthOfSegment);


	for (auto& [idx, Door] : IndexToDoorMapping)
	{
		if (idx > NewNumberOfWallSegments - 1)
		{
			ADoorActor* RemovedDoor = IndexToDoorMapping.FindAndRemoveChecked(idx);
			if (IsValid(RemovedDoor))
			{
				RemovedDoor->Destroy();
			}
		}
	}

	NumberOfWallSegments = NewNumberOfWallSegments;

	UpdateWall();
}


void AWallActor::HighLightBorder()
{

	for (auto& Temp : ArrayOfWallSegments)
	{
		if (IsValid(Temp))
		{
			Temp->SetRenderCustomDepth(true);
			Temp->CustomDepthStencilValue = 2;
		}

	}
}

void AWallActor::UnHighLightBorder()
{
	for (auto& Temp : ArrayOfWallSegments)
	{
		if (IsValid(Temp))
		{
			Temp->SetRenderCustomDepth(false);
		}

	}
}

void AWallActor::SetMaterial(FMaterialInfo MaterialInfo)
{
	Material = MaterialInfo.Material;

	UpdateWall();
}


void AWallActor::AttachDoorToComponent(UStaticMeshComponent* Component, ADoorActor* Door)
{
	int32 WallSegmentIdx = ArrayOfWallSegments.Find(Component);

	if (WallSegmentIdx != INDEX_NONE)
	{
		IndexToDoorMapping.Add({ WallSegmentIdx,Door });
	}

	UpdateWall();

	if (WallSegmentIdx < ArrayOfWallSegments.Num())
	{
		Door->AttachToComponent(ArrayOfWallSegments[WallSegmentIdx], FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("DoorSocket")));
		Door->SetActorRelativeLocation(FVector::ZeroVector);
		Door->SetActorRelativeRotation(FRotator::ZeroRotator);
	}



}

void AWallActor::DetachDoorFromComponent(UStaticMeshComponent* Component)
{

	int32 WallSegmentIdx = ArrayOfWallSegments.Find(Component);

	if (WallSegmentIdx != INDEX_NONE)
	{

		IndexToDoorMapping.Remove({ WallSegmentIdx });
	}


	UpdateWall();

	UnHighLightBorder();

}

void AWallActor::RemoveAllDoorsFromWall()
{
	for (auto& [idx, Door] : IndexToDoorMapping)
	{
		if (IsValid(Door))
		{
			Door->Destroy();
		}
	}

	IndexToDoorMapping.Empty();

	UpdateWall();

}

