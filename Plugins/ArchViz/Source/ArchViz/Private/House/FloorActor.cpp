// Fill out your copyright notice in the Description page of Project Settings.

#include "House/FloorActor.h"

#include "Components/SpinBox.h"
#include "Components/TextBlock.h"
#include "Widgets/ScrollableListWidget.h"

AFloorActor::AFloorActor()
{
  
    Dimensions = FVector(400, 400, 20);

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

    // Front
    Vertices.Add(FVector(0, Dimensions.Y, 0));
    Vertices.Add(FVector(0, Dimensions.Y, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, Dimensions.Y, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, Dimensions.Y, 0));

    // Back
    Vertices.Add(FVector(0, 0, 0));
    Vertices.Add(FVector(0, 0, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, 0, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, 0, 0));

    // Left
    Vertices.Add(FVector(0, 0, 0));
    Vertices.Add(FVector(0, 0, Dimensions.Z));
    Vertices.Add(FVector(0, Dimensions.Y, Dimensions.Z));
    Vertices.Add(FVector(0, Dimensions.Y, 0));

    // Right
    Vertices.Add(FVector(Dimensions.X, 0, 0));
    Vertices.Add(FVector(Dimensions.X, 0, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, Dimensions.Y, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, Dimensions.Y, 0));

    // Top
    Vertices.Add(FVector(0, Dimensions.Y, Dimensions.Z));
    Vertices.Add(FVector(0, 0, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, 0, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, Dimensions.Y, Dimensions.Z));

    // Bottom
    Vertices.Add(FVector(0, Dimensions.Y, 0));
    Vertices.Add(FVector(0, 0, 0));
    Vertices.Add(FVector(Dimensions.X, 0, 0));
    Vertices.Add(FVector(Dimensions.X, Dimensions.Y, 0));

    // Triangles (two triangles per face)
    Triangles.Append({
        0, 3, 1, 1, 3, 2,
        4, 5, 7, 5, 6, 7,
        8, 11, 9, 9, 11, 10,
        12, 13, 15, 13, 14, 15,
        16, 19, 17, 17, 19, 18,
        20, 21, 23, 21, 22, 23
        });

    // Normals (perpendicular to each face)
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

    // UV Mapping
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



    // Create the top plane section
    TArray<FVector> PlaneVertices;
    TArray<int32> PlaneTriangles;
    TArray<FVector> PlaneNormals;
    TArray<FVector2D> PlaneUVs;

    // Top Plane Vertices (same as top face of the cube)
    PlaneVertices.Add(FVector(0, Dimensions.Y, Dimensions.Z + 1));
    PlaneVertices.Add(FVector(0, 0, Dimensions.Z + 1));
    PlaneVertices.Add(FVector(Dimensions.X, 0, Dimensions.Z + 1));
    PlaneVertices.Add(FVector(Dimensions.X, Dimensions.Y, Dimensions.Z +1));

    // Top Plane Triangles
    PlaneTriangles.Append({ 0, 2,1 , 0, 3,2});

    // Top Plane Normals (pointing up)
    for (int32 i = 0; i < 4; i++)
    {
        PlaneNormals.Add(FVector(0, 0, 1));
    }

    // UV Mapping for the top plane
    PlaneUVs.Add(FVector2D(0, 1));
    PlaneUVs.Add(FVector2D(0, 0));
    PlaneUVs.Add(FVector2D(1, 0));
    PlaneUVs.Add(FVector2D(1, 1));


    ProcMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);


    if (IsValid(BottomMaterial))
    {
        if (!IsValid(DynBottomMaterial))
        {
            DynBottomMaterial = UMaterialInstanceDynamic::Create(BottomMaterial, this);
        }

        if (DynBottomMaterial)
        {
            DynBottomMaterial->SetVectorParameterValue(FName("Tiling/Offset"), FLinearColor(Dimensions.X / 400.0f, Dimensions.Y / 400.0f, 0, 0));
            ProcMesh->SetMaterial(0, DynBottomMaterial);

        }
    }


    ProcMesh->CreateMeshSection_LinearColor(1, PlaneVertices, PlaneTriangles, PlaneNormals, PlaneUVs, VertexColors, Tangents, true);



    if (IsValid(TopMaterial))
    {
        if (!IsValid(DynTopMaterial))
        {
            DynTopMaterial = UMaterialInstanceDynamic::Create(TopMaterial, this);
        }

        if (DynTopMaterial)
        {
            DynTopMaterial->SetVectorParameterValue(FName("Tiling/Offset"), FLinearColor(Dimensions.X / 400.0f, Dimensions.Y / 400.0f, 0, 0));
            ProcMesh->SetMaterial(1, DynTopMaterial);

        }
    }

}



  
  



void AFloorActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    GenerateFloor();
}

void AFloorActor::BeginPlay()
{
    Super::BeginPlay();

    if (IsValid(PropertyPanelUI))
    {
        PropertyPanelUI->SwitchToWidget(1);

        PropertyPanelUI->Title->SetText(FText::FromString(TEXT("Floor")));

        PropertyPanelUI->FloorLength->OnValueChanged.AddDynamic(this, &AFloorActor::OnDimensionsChange);
        PropertyPanelUI->FloorWidth->OnValueChanged.AddDynamic(this, &AFloorActor::OnDimensionsChange);
        PropertyPanelUI->FloorHeight->OnValueChanged.AddDynamic(this, &AFloorActor::OnDimensionsChange);

        PropertyPanelUI->FloorMaterialList->OnMaterialChange.AddDynamic(this, &AFloorActor::OnFloorMaterialChange);
        PropertyPanelUI->CeilingMaterialList->OnMaterialChange.AddDynamic(this, &AFloorActor::OnCeilingMaterialChange);

    }
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

void AFloorActor::SynchronizePropertyPanel()
{
    if(PropertyPanelUI)
    {
        PropertyPanelUI->FloorLength->SetValue(Dimensions.X);
        PropertyPanelUI->FloorWidth->SetValue(Dimensions.Y);
        PropertyPanelUI->FloorHeight->SetValue(Dimensions.Z);
    }
}

void AFloorActor::OnFloorMaterialChange(FMaterialInfo MaterialInfo)
{
    SetTopMaterial(MaterialInfo.Material);
    GenerateFloor();
}

void AFloorActor::OnCeilingMaterialChange(FMaterialInfo MaterialInfo)
{
    SetBottomMaterial(MaterialInfo.Material);
    GenerateFloor();
}

void AFloorActor::OnDimensionsChange(float X)
{
    Dimensions.X = PropertyPanelUI->FloorLength->GetValue();
    Dimensions.Y = PropertyPanelUI->FloorWidth->GetValue();
    Dimensions.Z = PropertyPanelUI->FloorHeight->GetValue();

    GenerateFloor();

    HighLightBorder();
}

// Getters
const FVector& AFloorActor::GetDimensions() const
{
    return Dimensions;
}

UMaterialInterface* AFloorActor::GetBottomMaterial() const
{
    return BottomMaterial;
}

UMaterialInterface* AFloorActor::GetTopMaterial() const
{
    return TopMaterial;
}


// Setters

void AFloorActor::SetDimensions(const FVector& InDimensions)
{
    Dimensions = InDimensions;
}

void AFloorActor::SetBottomMaterial(UMaterialInterface* InMaterial)
{
    BottomMaterial = InMaterial;

    if (IsValid(BottomMaterial))
    {
        UE_LOG(LogTemp, Warning, TEXT("OK"));
        DynBottomMaterial = UMaterialInstanceDynamic::Create(BottomMaterial, this);

        if (DynBottomMaterial)
        {
            DynBottomMaterial->SetVectorParameterValue(FName("Tiling/Offset"), FLinearColor(Dimensions.X / 400.0f, Dimensions.Y / 400.0f, 0, 0));
            ProcMesh->SetMaterial(0, DynBottomMaterial);
        }
    }


}

void AFloorActor::SetTopMaterial(UMaterialInterface* InMaterial)
{
    TopMaterial = InMaterial;

    if (IsValid(TopMaterial))
    {
    	DynTopMaterial = UMaterialInstanceDynamic::Create(TopMaterial, this);
        if (DynTopMaterial)
        {
            FVector2D TilingFactor = FVector2D(Dimensions.X / 400.0f, Dimensions.Y / 400.0f);
            DynTopMaterial->SetVectorParameterValue(FName("Tiling/Offset"), FLinearColor(TilingFactor.X, TilingFactor.Y, 0, 0));
            ProcMesh->SetMaterial(1, DynTopMaterial);
        }
    }
}
