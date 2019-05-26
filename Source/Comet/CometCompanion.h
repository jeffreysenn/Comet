// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MoodComponent.h"

#include "CometCompanion.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCompanionDelegate);

UCLASS()
class COMET_API ACometCompanion : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACometCompanion();

public:
	UPROPERTY(Category = Collider, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* BrakeShpere;

	UPROPERTY(Category = Collider, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SyncSphere;

	UPROPERTY(Category = Collider, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CameraLockSphere;

	UPROPERTY(Category = Beat, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBeatComponent* BeatComponent;

	UPROPERTY(Category = Billboard, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMaterialBillboardComponent* MaterialBillboard;

	bool bIsFree = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMoodEnum MoodType;

	UPROPERTY(BlueprintAssignable)
	FCompanionDelegate OnCompanionSetFree;

protected:
	UPROPERTY(Category = Particle, EditAnywhere, BlueprintReadWrite)
	class UNiagaraSystem* BeatParticleTemplate;

	UPROPERTY(Category = Particle, EditAnywhere, BlueprintReadWrite)
	FLinearColor Colour = FLinearColor(1, 1, 1, 1);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnSetFree(AActor* Liberator);

	UFUNCTION(BlueprintCallable)
	class UMoodComponent* FindLiberatorMoodComp(AActor* Liberator);

public:	
	UFUNCTION()
	void OnBrakeSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCameraLockSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCameraLockSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void RespondToBeatPlayed();

	UFUNCTION(BlueprintCallable)
	virtual UNiagaraComponent* SpawnParticle(UNiagaraSystem* SystemTemplate);

	UFUNCTION()
	void RespondToAllBeatsMatched();

	UFUNCTION(BlueprintCallable)
	virtual void SetCometCompanionFree(AActor* Liberator);

	UFUNCTION()
	void DestoryNS(class UNiagaraComponent* NSToDestroy);

};
