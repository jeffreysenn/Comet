// Fill out your copyright notice in the Description page of Project Settings.

#include "CometProceduralMeshComponent.h"


void UCometProceduralMeshComponent::GenerateCube() 
{
	//UE_LOG(LogTemp, Warning, TEXT("cube was generated"));
	GenerateCubeMesh(100);	
}

void UCometProceduralMeshComponent::ResizeGrowth(int32 dustNum)
{
	//Triangles[index] += dustNum;//FVector(dustNum, dustNum, dustNum);
	Vertices[0] += FVector(0, -dustNum, 0); //lower left - 0
	Vertices[1] += FVector(0, -dustNum, dustNum); //upper left - 1

	Vertices[2] += FVector(0, dustNum, 0); //lower right - 2 
	Vertices[3] += FVector(0, dustNum, dustNum); //upper right - 3

	Vertices[4] += FVector(dustNum, -dustNum, 0); //lower front left - 4
	Vertices[5] += FVector(dustNum, -dustNum, dustNum); //upper front left - 5

	Vertices[6] += FVector(dustNum, dustNum, dustNum); //upper front right - 6
	Vertices[7] += FVector(dustNum, dustNum, 0); //lower front right - 7
	UpdateMeshSection(0, Vertices, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>());
}

void UCometProceduralMeshComponent::ResizeShrink(int32 dustNum) //could also merge these functions together because then we could a bool or something for Bis_shrinking = true;
{
	//Triangles[index] += dustNum;//FVector(dustNum, dustNum, dustNum);
	Vertices[0] -= FVector(0, -dustNum, 0); //lower left - 0
	Vertices[1] -= FVector(0, -dustNum, dustNum); //upper left - 1

	Vertices[2] -= FVector(0, dustNum, 0); //lower right - 2 
	Vertices[3] -= FVector(0, dustNum, dustNum); //upper right - 3

	Vertices[4] -= FVector(dustNum, -dustNum, 0); //lower front left - 4
	Vertices[5] -= FVector(dustNum, -dustNum, dustNum); //upper front left - 5

	Vertices[6] -= FVector(dustNum, dustNum, dustNum); //upper front right - 6
	Vertices[7] -= FVector(dustNum, dustNum, 0); //lower front right - 7
	UpdateMeshSection(0, Vertices, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>());
}

void UCometProceduralMeshComponent::MoveRenderedObject(FVector distance)
{
	//Moving the mesh
	for (int32 index = 0; index < Vertices.Num(); index++)
	{
		Vertices[index] += distance;
	}
	UpdateMeshSection(0, Vertices, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>());
}

void UCometProceduralMeshComponent::addCube(int32 location) //size relative to the actor/pawn
{
	GenerateCubeMesh(location);
	//UE_LOG(LogTemp, Warning, TEXT("new cube was generated"));
}

void UCometProceduralMeshComponent::AddTriangle(int32 V1, int32 V2, int32 V3)
{
	Triangles.Add(V1);
	Triangles.Add(V2);
	Triangles.Add(V3);
}

void UCometProceduralMeshComponent::GenerateCubeMesh(int32 location)
{
	//6 sides on cube, 4 verts each (corners)

	//These are relative locations to the placed Actor in the world aka size
	Vertices.Add(FVector(0, -location, 0)); //lower left - 0
	Vertices.Add(FVector(0, -location, location)); //upper left - 1

	Vertices.Add(FVector(0, location, 0)); //lower right - 2 
	Vertices.Add(FVector(0, location, location)); //upper right - 3

	Vertices.Add(FVector(location, -location, 0)); //lower front left - 4
	Vertices.Add(FVector(location, -location, location)); //upper front left - 5

	Vertices.Add(FVector(location, location, location)); //upper front right - 6
	Vertices.Add(FVector(location, location, 0)); //lower front right - 7

	//Back face of cube
	AddTriangle(0, 2, 3);
	AddTriangle(3, 1, 0);

	//Left face of cube
	AddTriangle(0, 1, 4);
	AddTriangle(4, 1, 5);

	//Front face of cube
	AddTriangle(4, 5, 7);
	AddTriangle(7, 5, 6);

	//Right face of cube
	AddTriangle(7, 6, 3);
	AddTriangle(3, 2, 7);

	//Top face
	AddTriangle(1, 3, 5);
	AddTriangle(6, 5, 3);

	//bottom face
	AddTriangle(2, 0, 4);
	AddTriangle(4, 7, 2);

	
	VertexColors.Add(FLinearColor(0.f, 0.f, 1.f));
	VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(0.5f, 1.f, 0.5f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(1.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 1.f));

	CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);
}
