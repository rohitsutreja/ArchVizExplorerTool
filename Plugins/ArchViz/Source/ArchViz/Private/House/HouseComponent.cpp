// Fill out your copyright notice in the Description page of Project Settings.


#include "House/HouseComponent.h"

#include "Components/Button.h"

void AHouseComponent::SnapActorToGrid(const FVector& GridSize)
{
	FVector ActorLocation = GetActorLocation();

	ActorLocation.X = FMath::RoundToFloat(ActorLocation.X / GridSize.X) * GridSize.X;
	ActorLocation.Y = FMath::RoundToFloat(ActorLocation.Y / GridSize.Y) * GridSize.Y;
	ActorLocation.Z = FMath::RoundToFloat(ActorLocation.Z / GridSize.Z) * GridSize.Z;

	SetActorLocation(ActorLocation);
}

void AHouseComponent::Rotate()
{
	FRotator CurrentRotation = GetActorRotation();

	FRotator NewRotation{ CurrentRotation };

	NewRotation.Yaw = StaticCast<int32>(CurrentRotation.Yaw + 90) % 360;

	SetActorRotation(NewRotation);
}

void AHouseComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(EndPlayReason == EEndPlayReason::Type::Destroyed)
	{
		TArray<USceneComponent*> ChildActors;

		RootComponent->GetChildrenComponents(true, ChildActors);

		for (auto& ChildActor : ChildActors)
		{
			UE_LOG(LogTemp, Warning, TEXT("Inside"));
			ChildActor->GetAttachParentActor()->Destroy();
		}
	}

	Super::EndPlay(EndPlayReason);
}

