// Fill out your copyright notice in the Description page of Project Settings.


#include "Roads/RoadSplineActor.h"

#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

ARoadSplineActor::ARoadSplineActor()
{
	Spline = CreateDefaultSubobject<USplineComponent>("RoadSpline");
	RootComponent = Spline;

	

}

void ARoadSplineActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

   // UpdateRoad();

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


void ARoadSplineActor::UpdateRoad()
{
    DestroyRoad();


    Spline->SetSplinePoints(SplinePoints, ESplineCoordinateSpace::World);

    const int32 NumPoints = Spline->GetNumberOfSplinePoints();

    for (int32 i = 0; i < NumPoints; i+=2)
    {
    //   Spline->SetSplinePointType(i, ESplinePointType::Linear, true);
    }


    FVector MeshBounds = SplineMesh->GetBounds().BoxExtent * 2.0f;

    float DesiredSegmentLength = MeshBounds.X * 0.25;
    float SplineLength = Spline->GetSplineLength();
    int32 NumberOfSegments = FMath::Floor(SplineLength / DesiredSegmentLength);
    float MaxTangentLength = DesiredSegmentLength * 0.5;

    for (int32 i = 0; i <= NumberOfSegments; ++i)
    {
        FVector StartLocation, StartTangent, EndLocation, EndTangent;
        float StartDistance = i * DesiredSegmentLength;
        float EndDistance = FMath::Min((i + 1) * DesiredSegmentLength, SplineLength);

        StartLocation = Spline->GetLocationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);
        StartTangent = Spline->GetTangentAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local).GetClampedToMaxSize(MaxTangentLength);;
        EndLocation = Spline->GetLocationAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);
        EndTangent = Spline->GetTangentAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local).GetClampedToMaxSize(MaxTangentLength);;

        USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
        SplineMeshComponent->SetMobility(EComponentMobility::Movable);
        SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
        SplineMeshComponent->SetRelativeLocation(FVector(0, 0, 20));
        SplineMeshComponent->SetStaticMesh(SplineMesh);
        SplineMeshComponent->RegisterComponent();
        SplineMeshComponent->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent);

        SplineMeshComponents.Add(SplineMeshComponent);
    }
}



void ARoadSplineActor::UpdateRoad2()
{
    DestroyRoad();


    Spline->SetSplinePoints(SplinePoints, ESplineCoordinateSpace::World);

    const int32 NumPoints = Spline->GetNumberOfSplinePoints();

    if(NumPoints<2)
    {
        return;
    }
    for (int32 i = 0; i < NumPoints; i++)
    {
       Spline->SetSplinePointType(i, ESplinePointType::Linear, true);
    }

    for (int32 i = 0; i < NumPoints - 1; ++i)
    {
        FVector StartLocation, EndLocation ;
        FVector StartTangent, EndTangent;

        StartTangent = Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
        EndTangent = Spline->GetTangentAtSplinePoint(i+1, ESplineCoordinateSpace::Local);


        StartLocation = Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
        EndLocation = Spline->GetLocationAtSplinePoint(i+1, ESplineCoordinateSpace::Local);

        

        auto Tangent = UKismetMathLibrary::GetDirectionUnitVector(StartLocation, EndLocation);


        Tangent = Spline->GetDirectionAtSplinePoint(i,ESplineCoordinateSpace::Local) * 200;

        USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
        SplineMeshComponent->SetMobility(EComponentMobility::Movable);
        SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
       SplineMeshComponent->SetStartOffset({-200,0});
       SplineMeshComponent->SetEndOffset({ -200,0});


    	SplineMeshComponent->SetRelativeLocation(FVector(0, 0, 20));

        SplineMeshComponent->SetStaticMesh(SplineMesh);
        SplineMeshComponent->RegisterComponent();

 

        SplineMeshComponent->SetStartAndEnd(StartLocation - Tangent, Tangent, EndLocation + Tangent, Tangent);


        SplineMeshComponents.Add(SplineMeshComponent);
    }
}
