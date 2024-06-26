// Fill out your copyright notice in the Description page of Project Settings.


#include "Interior/InteriorActor.h"

#include "Components/TextBlock.h"
#include "Widgets/PropertyPanelWidget.h"

AInteriorActor::AInteriorActor()
{

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());

}

void AInteriorActor::HighLightBorder()
{
	StaticMeshComponent->SetRenderCustomDepth(true);
	StaticMeshComponent->CustomDepthStencilValue = 2;
}

void AInteriorActor::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(PropertyPanelUI))
	{
		PropertyPanelUI->SwitchToWidget(4);

		PropertyPanelUI->Title->SetText(FText::FromString(TEXT("Interior")));

	}
}

void AInteriorActor::UnHighLightBorder()
{
	StaticMeshComponent->SetRenderCustomDepth(false);
}


void AInteriorActor::SetStaticMesh(UStaticMesh* StaticMesh)
{
	this->CurrentStaticMesh = StaticMesh;
	StaticMeshComponent->SetStaticMesh(this->CurrentStaticMesh);
}


void AInteriorActor::SnapActorToGrid(const FVector& GridSize)
{
	FVector ActorLocation = GetActorLocation();

	ActorLocation.X = FMath::RoundToFloat(ActorLocation.X / GridSize.X) * GridSize.X;
	ActorLocation.Y = FMath::RoundToFloat(ActorLocation.Y / GridSize.Y) * GridSize.Y;
	ActorLocation.Z = FMath::RoundToFloat(ActorLocation.Z / GridSize.Z) * GridSize.Z;

	SetActorLocation(ActorLocation);
}



void AInteriorActor::Rotate()
{
	FRotator CurrentRotation = GetActorRotation();

	FRotator NewRotation{ CurrentRotation };

	NewRotation.Yaw = StaticCast<int32>(CurrentRotation.Yaw + 90) % 360;

	SetActorRotation(NewRotation);
}

