// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoodComponent.h"

#include "Crystal.generated.h"

UCLASS()
class COMET_API ACrystal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrystal();

public:
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CrystalMesh;

	UPROPERTY(Category = Collision, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* AttractionSphere;

	UPROPERTY(Category = Billboard, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMaterialBillboardComponent* GlowBillboard;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	 TArray<class UStaticMesh*> MeshArr;

	UPROPERTY(BlueprintReadOnly)
	class AActor* Obtainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMoodEnum MoodType = EMoodEnum::ME_DefaultType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Mass = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashCharge = 0.1f;

protected:
	void BeginPlay() override;

	UFUNCTION()
	void OnAttractionSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void ShinePlease();
};
