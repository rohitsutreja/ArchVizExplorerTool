// Fill out your copyright notice in the Description page of Project Settings.


#include "House/FloorActor.h"

#include "Components/SpinBox.h"
#include "Components/TextBlock.h"


AFloorActor::AFloorActor()
{
    Length = 400;
    Width = 400;
    Height = 20;

    ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMesh"));
    RootComponent = ProcMesh;
}


void AFloorActor::GenerateFloor()
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	FVector Dimensions(Length, Width, Height);

	//Front
	Vertices.Add(FVector(0, Dimensions.Y, 0));
	Vertices.Add(FVector(0, Dimensions.Y, Dimensions.Z));
	Vertices.Add(FVector(Dimensions.X, Dimensions.Y, Dimensions.Z));
	Vertices.Add(FVector(Dimensions.X, Dimensions.Y, 0));

	//Back
	Vertices.Add(FVector(0, 0, 0));
	Vertices.Add(FVector(0, 0, Dimensions.Z));
	Vertices.Add(FVector(Dimensions.X, 0, Dimensions.Z));
	Vertices.Add(FVector(Dimensions.X, 0, 0));


	//Left
	Vertices.Add(FVector(0, 0, 0));
	Vertices.Add(FVector(0, 0, Dimensions.Z));
	Vertices.Add(FVector(0, Dimensions.Y, Dimensions.Z));
	Vertices.Add(FVector(0, Dimensions.Y, 0));

	//Right
	Vertices.Add(FVector(Dimensions.X, 0, 0));
	Vertices.Add(FVector(Dimensions.X, 0, Dimensions.Z));
	Vertices.Add(FVector(Dimensions.X, Dimensions.Y, Dimensions.Z));
	Vertices.Add(FVector(Dimensions.X, Dimensions.Y, 0));

	//Top
	Vertices.Add(FVector(0, Dimensions.Y, Dimensions.Z));
	Vertices.Add(FVector(0, 0, Dimensions.Z));
	Vertices.Add(FVector(Dimensions.X, 0, Dimensions.Z));
	Vertices.Add(FVector(Dimensions.X, Dimensions.Y, Dimensions.Z));

	//Bottom
	Vertices.Add(FVector(0, Dimensions.Y, 0));
	Vertices.Add(FVector(0, 0, 0));
	Vertices.Add(FVector(Dimensions.X, 0, 0));
	Vertices.Add(FVector(Dimensions.X, Dimensions.Y, 0));


	// triangles (two triangles per face)
	Triangles.Append({
		0, 3, 1, 1, 3, 2,
		4, 5, 7, 5, 6, 7,
		8, 11, 9, 9, 11, 10,
		12, 13, 15, 13, 14, 15,
		16, 19, 17, 17, 19, 18,
		20, 21, 23, 21, 22, 23
		});

	// normals (perpendicular to each face)

	for (int32 i{}; i < 4; i++)
	{
		Normals.Add(FVector(0, 1, 0));
	}
	for (int32 i{}; i < 4; i++)
	{
		Normals.Add(FVector(0, -1, 0));
	}
	for (int32 i{}; i < 4; i++)
	{
		Normals.Add(FVector(-1, 0, 0));
	}
	for (int32 i{}; i < 4; i++)
	{
		Normals.Add(FVector(1, 0, 0));
	}
	for (int32 i{}; i < 4; i++)
	{
		Normals.Add(FVector(0, 0, 1));
	}

	for (int32 i{}; i < 4; i++)
	{
		Normals.Add(FVector(0, 0, -1));
	}


	//UV Mapping
	for (int32 i{}; i < 3; ++i)
	{
		UVs.Add(FVector2D(0, 1));
		UVs.Add(FVector2D(0, 0));
		UVs.Add(FVector2D(1, 0));
		UVs.Add(FVector2D(1, 1));

		UVs.Add(FVector2D(0, 1));
		UVs.Add(FVector2D(0, 0));
		UVs.Add(FVector2D(-1, 0));
		UVs.Add(FVector2D(-1, 1));
	}



	ProcMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);


}

void AFloorActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

    GenerateFloor();
}


void AFloorActor::HighLightBorder()
{
	ProcMesh->SetRenderCustomDepth(true);
	ProcMesh->CustomDepthStencilValue = 2;
}

void AFloorActor::UnHighLightBorder()
{
	ProcMesh->SetRenderCustomDepth(false);
}

void AFloorActor::UpdateDimensions(float X)
{
	Length = PropertyPanelUI->FloorLength->GetValue();
	Width = PropertyPanelUI->FloorWidth->GetValue();
	Height = PropertyPanelUI->FloorHeight->GetValue();

	GenerateFloor();

	HighLightBorder();
}

void AFloorActor::BeginPlay()
{
	Super::BeginPlay();
	if(IsValid(PropertyPanelUI))
	{
		PropertyPanelUI->Title->SetText(FText::FromString(TEXT("Floor")));


		PropertyPanelUI->SwitchToWidget(1);

		PropertyPanelUI->FloorLength->OnValueChanged.AddDynamic(this, &AFloorActor::UpdateDimensions);
		PropertyPanelUI->FloorWidth->OnValueChanged.AddDynamic(this, &AFloorActor::UpdateDimensions);
		PropertyPanelUI->FloorHeight->OnValueChanged.AddDynamic(this, &AFloorActor::UpdateDimensions);

	}

}
