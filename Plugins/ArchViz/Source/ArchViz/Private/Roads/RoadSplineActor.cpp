// Fill out your copyright notice in the Description page of Project Settings.


#include "Roads/RoadSplineActor.h"

#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/SpinBox.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"

ARoadSplineActor::ARoadSplineActor()
{
	Spline = CreateDefaultSubobject<USplineComponent>("RoadSpline");
	RootComponent = Spline;
}

void ARoadSplineActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}

void ARoadSplineActor::DestroyRoad()
{
	for (int i = 0; i < SplineMeshComponents.Num(); i++) {
		if (SplineMeshComponents[i]) {
			SplineMeshComponents[i]->DestroyComponent();
		}
	}
	SplineMeshComponents.Empty();
}


void ARoadSplineActor::AddSplinePoint(const FVector& Location)
{
	if (!SplinePoints.IsEmpty())
	{
		if (UKismetMathLibrary::Vector_Distance(Location, SplinePoints[SplinePoints.Num() - 1]) < 400)
		{
			return;
		}

	}
	SplinePoints.Add(Location);
}

void ARoadSplineActor::UpdateRoad()
{
	DestroyRoad();

	if (SplinePoints.Num() < 2)
	{
		return;
	}

	Spline->SetSplinePoints(SplinePoints, ESplineCoordinateSpace::World);

	const int32 NumPoints = Spline->GetNumberOfSplinePoints();

	FVector MeshBounds = SplineMesh->GetBounds().BoxExtent * 2.0f;

	float MeshWidth = MeshBounds.Y;

	float MeshLength = MeshBounds.X;

	float ScaleFactor = Width / MeshWidth;


	if (TypeOfRoad == ERoadType::Sharp)
	{
		for (int32 i{}; i < NumPoints; ++i)
		{
			Spline->SetSplinePointType(i, ESplinePointType::CurveClamped, true);
		}
	}

	for (int32 i = 0; i < NumPoints - 1; ++i)
	{
		float StartDistance = Spline->GetDistanceAlongSplineAtSplinePoint(i);
		float EndDistance = Spline->GetDistanceAlongSplineAtSplinePoint(i + 1);
		float SegmentLength = EndDistance - StartDistance;

		int32 NumberOfSegments = SegmentLength / MeshLength;

		if (NumberOfSegments > 0)
		{
			float StrechedMeshLength = SegmentLength / NumberOfSegments;


			for (int32 j = 0; j < NumberOfSegments; ++j)
			{
				FVector StartLocation, StartTangent, EndLocation, EndTangent;
				float LocalStartDistance = StartDistance + j * StrechedMeshLength - 200;
				float LocalEndDistance = StartDistance + (j + 1) * StrechedMeshLength - 200;

				StartLocation = Spline->GetLocationAtDistanceAlongSpline(LocalStartDistance, ESplineCoordinateSpace::Local);
				StartTangent = Spline->GetTangentAtDistanceAlongSpline(LocalStartDistance, ESplineCoordinateSpace::Local).GetClampedToMaxSize(StrechedMeshLength);
				EndLocation = Spline->GetLocationAtDistanceAlongSpline(LocalEndDistance, ESplineCoordinateSpace::Local);
				EndTangent = Spline->GetTangentAtDistanceAlongSpline(LocalEndDistance, ESplineCoordinateSpace::Local).GetClampedToMaxSize(StrechedMeshLength);

				USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
				SplineMeshComponent->SetMobility(EComponentMobility::Movable);
				SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
				SplineMeshComponent->SetStaticMesh(SplineMesh);
				SplineMeshComponent->RegisterComponent();
				SplineMeshComponent->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent);
				SplineMeshComponent->SetStartScale({ ScaleFactor,1 }); SplineMeshComponent->SetEndScale({ ScaleFactor,1 });

				SplineMeshComponents.Add(SplineMeshComponent);
			}
		}

	}

}

void ARoadSplineActor::OnDeleteButtonDown()
{
	SplinePoints.Empty();

	Destroy();
}

void ARoadSplineActor::OnWidthChanged(float InValue)
{
	Width = InValue;

	UpdateRoad();
}

void ARoadSplineActor::OnRoadTypeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectedItem == TEXT("Curved"))
	{
		TypeOfRoad = ERoadType::Curve;
	}
	else if (SelectedItem == TEXT("Sharp"))
	{
		TypeOfRoad = ERoadType::Sharp;
	}

	UpdateRoad();
}

void ARoadSplineActor::BeginPlay()
{
	Super::BeginPlay();

	PropertyPanelUI = CreateWidget<UPropertyPanelWidget>(GetWorld(), PropertyPanelClass);

	if (IsValid(PropertyPanelUI))
	{
		PropertyPanelUI->DeleteWallButton->OnClicked.AddDynamic(this, &ARoadSplineActor::OnDeleteButtonDown);

		PropertyPanelUI->RoadWidthValue->OnValueChanged.AddDynamic(this, &ARoadSplineActor::OnWidthChanged);

		PropertyPanelUI->RoadType->OnSelectionChanged.AddDynamic(this, &ARoadSplineActor::OnRoadTypeChanged);

		PropertyPanelUI->Title->SetText(FText::FromString(TEXT("Road")));

		PropertyPanelUI->SwitchToWidget(3);

		PropertyPanelUI->AddToViewport(0);
	}
}

void ARoadSplineActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	if (IsValid(PropertyPanelUI))
	{
		PropertyPanelUI->RemoveFromParent();
	}

	Super::EndPlay(EndPlayReason);
}
