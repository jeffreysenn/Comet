// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "RotatingMesh.generated.h"

/**
 * 
 */
UCLASS()
class COMET_API ARotatingMesh : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	ARotatingMesh();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MakeEditWidget = true), Category = Rotation)
	FVector RotationCentre;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation)
	FRotator RotationRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation)
	bool bSweep = true;

public:
	virtual void Tick(float DeltaSeconds) override;
};
