// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CometCompanion.h"
#include "SunCompanion.generated.h"

/**
 * 
 */

UCLASS()
class COMET_API ASunCompanion : public ACometCompanion
{
	GENERATED_BODY()

public:
	ASunCompanion();

public:
	UPROPERTY(Category = Particle, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent* NormalParticle;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSyncSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSyncSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void SetCometCompanionFree(AActor* Liberator) override;

};
