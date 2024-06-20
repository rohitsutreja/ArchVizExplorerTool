// Fill out your copyright notice in the Description page of Project Settings.


#include "House/WallActor.h"

#include "ComponentUtils.h"
#include "Components/SpinBox.h"
#include "Components/TextBlock.h"
#include "House/DoorActor.h"

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

	if(IsValid(PropertyPanelUI))
	{
		PropertyPanelUI->SwitchToWidget(0);

		PropertyPanelUI->Title->SetText(FText::FromString(TEXT("Wall")));

		PropertyPanelUI->WallLengthValue->OnValueChanged.AddDynamic(this, &AWallActor::SetLength);
	}

	UpdateWall();
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
		ArrayOfWallSegments.Add(WallSegment);
	}




	for (auto& [idx, Door] : IndexToDoorMapping)
	{

		if (idx < ArrayOfWallSegments.Num())
		{
			ArrayOfWallSegments[idx]->SetStaticMesh(DoorHallSegment);
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
			RemovedDoor->Destroy();
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



void AWallActor::AttachDoorToComponent(UStaticMeshComponent* Component, ADoorActor* Door)
{
	int32 WallSegmentIdx = ArrayOfWallSegments.Find(Component);

	if(WallSegmentIdx != INDEX_NONE)
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
		IndexToDoorMapping.Remove({ WallSegmentIdx});
	}

	UpdateWall();

}

void AWallActor::RemoveAllDoorsFromWall()
{
	for(auto& [idx,Door] : IndexToDoorMapping)
	{
		Door->Destroy();
	}

	IndexToDoorMapping.Empty();

	UpdateWall();

}

void AWallActor::Destroyed()
{
	RemoveAllDoorsFromWall();

	Super::Destroyed();
}
