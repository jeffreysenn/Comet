// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CometCompanion.h"
#include "SunCompanion.generated.h"

USTRUCT(BlueprintType)
struct FSunCompanionColourStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DesaturationFraction = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeshEmissive = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BillboardEmissive = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor ParticleColour = FLinearColor(1,1,1,1);

	FSunCompanionColourStruct()
	{
	}
};

UCLASS()
class COMET_API ASunCompanion : public ACometCompanion
{
	GENERATED_BODY()

public:
	ASunCompanion();

protected:
	UPROPERTY(EditAnywhere)
	TArray<FSunCompanionColourStruct> Colours;

	int32 ColourIndex = 0;

public:
	UPROPERTY(Category = Particle, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent* NormalParticle;

public:
	UFUNCTION()
	void RespondToOnCompanionSetFree();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSyncSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSyncSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void SetCometCompanionFree(AActor* Liberator) override;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateColour(FSunCompanionColourStruct OldColour, FSunCompanionColourStruct NewColour);

	TArray<class ACometCompanion*> FindAllOtherCompanions();

};
