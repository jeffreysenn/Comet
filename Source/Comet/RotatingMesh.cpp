// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatingMesh.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"

ARotatingMesh::ARotatingMesh()
{
	PrimaryActorTick.bCanEverTick = true;

	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);

	GetStaticMeshComponent()->SetUseCCD(true);

}

void ARotatingMesh::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (RotationRate.IsZero())
	{
		return;
	}

	// Compute new rotation
	const FQuat OldRotation = GetActorQuat();
	const FQuat DeltaRotation = (RotationRate * DeltaSeconds).Quaternion();
	const FQuat NewRotation = OldRotation * DeltaRotation;

	// Compute new location
	FVector DeltaLocation = FVector::ZeroVector;
	if (!RotationCentre.IsZero())
	{
		const FVector OldPivot = OldRotation.RotateVector(RotationCentre);
		const FVector NewPivot = NewRotation.RotateVector(RotationCentre);
		DeltaLocation = (OldPivot - NewPivot); 
	}

	FVector NewLocation = GetActorLocation() + DeltaLocation;


	SetActorLocationAndRotation(NewLocation, NewRotation, bSweep);
}
