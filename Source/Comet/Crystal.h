// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MoodComponent.h"

#include "Crystal.generated.h"

UCLASS()
class COMET_API ACrystal : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrystal();

public:
	UPROPERTY(Category = Collision, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* AttractionSphere;

	UPROPERTY(Category = Billboard, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMaterialBillboardComponent* GlowBillboard;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	 TArray<UStaticMesh*> MeshArr;

	UPROPERTY(BlueprintReadOnly)
	class AActor* Obtainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMoodEnum MoodType = EMoodEnum::ME_DefaultType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Mass = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashCharge = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D MinMaxScale = FVector2D(8.f, 12.f);

protected:
	 virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void OnAttractionSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void ShinePlease();

	FVector GetRandomScale(float MinScale, float MaxScale);
};
