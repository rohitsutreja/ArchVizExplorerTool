// Fill out your copyright notice in the Description page of Project Settings.


#include "Roads/RoadSplineActor.h"

#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/SpinBox.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"
#include "Managers/RoadConstructionManager.h"
#include "Widgets/RoadConstructionWidget.h"
#include "Widgets/ScrollableListWidget.h"

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
	if (SplinePoints.Num() < 2)
	{
		DestroyRoad();
		return;
	}

	Spline->SetSplinePoints(SplinePoints, ESplineCoordinateSpace::World);

	const int32 NumPoints = Spline->GetNumberOfSplinePoints();
	const FVector MeshBounds = SplineMesh->GetBounds().BoxExtent * 2.0f;
	const float MeshWidth = MeshBounds.Y;
	const float MeshLength = MeshBounds.X;
	const float ScaleFactor = Width / MeshWidth;

	if (TypeOfRoad == ERoadType::Sharp)
	{
		for (int32 i = 0; i < NumPoints; ++i)
		{
			Spline->SetSplinePointType(i, ESplinePointType::CurveClamped, true);
		}
	}

	int32 SegmentIndex = 0;

	for (int32 i = 0; i < NumPoints - 1; ++i)
	{
		const float StartDistance = Spline->GetDistanceAlongSplineAtSplinePoint(i);
		const float EndDistance = Spline->GetDistanceAlongSplineAtSplinePoint(i + 1);
		const float SegmentLength = EndDistance - StartDistance;
		const int32 NumberOfSegments = FMath::CeilToInt(SegmentLength / MeshLength);

		for (int32 j = 0; j < NumberOfSegments; ++j)
		{
			if (SegmentIndex >= SplineMeshComponents.Num())
			{
				USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
				SplineMeshComponent->SetMobility(EComponentMobility::Movable);
				SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
				SplineMeshComponent->SetStaticMesh(SplineMesh);
				SplineMeshComponent->RegisterComponent();

				SplineMeshComponent->SetRenderCustomDepth(true);
				SplineMeshComponent->CustomDepthStencilValue = 2;

				SplineMeshComponents.Add(SplineMeshComponent);
			}

			USplineMeshComponent* SplineMeshComponent = SplineMeshComponents[SegmentIndex];

			SplineMeshComponent->SetVisibility(true);

			float StrechedMeshLength = SegmentLength / NumberOfSegments;

			float LocalStartDistance = StartDistance + j * StrechedMeshLength - 200;
			float LocalEndDistance = StartDistance + (j + 1) * StrechedMeshLength - 200;

			const FVector StartLocation = Spline->GetLocationAtDistanceAlongSpline(LocalStartDistance, ESplineCoordinateSpace::Local);
			const FVector StartTangent = Spline->GetTangentAtDistanceAlongSpline(LocalStartDistance, ESplineCoordinateSpace::Local).GetClampedToMaxSize(MeshLength);
			const FVector EndLocation = Spline->GetLocationAtDistanceAlongSpline(LocalEndDistance, ESplineCoordinateSpace::Local);
			const FVector EndTangent = Spline->GetTangentAtDistanceAlongSpline(LocalEndDistance, ESplineCoordinateSpace::Local).GetClampedToMaxSize(MeshLength);

			SplineMeshComponent->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent, true);
			SplineMeshComponent->SetStartScale(FVector2D(ScaleFactor, 1));
			SplineMeshComponent->SetEndScale(FVector2D(ScaleFactor, 1));
			SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			SplineMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
			if(IsValid(Material))
			{
				SplineMeshComponent->SetMaterial(0, Material);
			}

			++SegmentIndex;
		}
	}

	for (int32 i = SegmentIndex; i < SplineMeshComponents.Num(); ++i)
	{
		SplineMeshComponents[i]->SetVisibility(false);
	}
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

	if (IsValid(PropertyPanelUI))
	{
		PropertyPanelUI->SwitchToWidget(3);

		PropertyPanelUI->Title->SetText(FText::FromString(TEXT("Road")));

		PropertyPanelUI->RoadWidthValue->OnValueChanged.AddDynamic(this, &ARoadSplineActor::OnWidthChanged);

		PropertyPanelUI->RoadType->OnSelectionChanged.AddDynamic(this, &ARoadSplineActor::OnRoadTypeChanged);

		PropertyPanelUI->RoadMaterialList->OnMaterialChange.AddDynamic(this, &ARoadSplineActor::SetMaterial);





	}
}

void ARoadSplineActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	if (IsValid(PropertyPanelUI))
	{
		PropertyPanelUI->RemoveFromParent();
	}

	SplinePoints.Empty();

	OnRoadDelete.ExecuteIfBound();

	Super::EndPlay(EndPlayReason);
}

void ARoadSplineActor::HighLightBorder()
{
	for(auto& comp : SplineMeshComponents)
	{
		comp->SetRenderCustomDepth(true);
		comp->CustomDepthStencilValue = 2;
	}

	UpdateRoad();
}

void ARoadSplineActor::UnHighLightBorder()
{
	for (auto& comp : SplineMeshComponents)
	{
		comp->SetRenderCustomDepth(false);
	}
}

void ARoadSplineActor::SetMaterial(FMaterialInfo MaterialInfo)
{
	Material = MaterialInfo.Material;
	UpdateRoad();
}



