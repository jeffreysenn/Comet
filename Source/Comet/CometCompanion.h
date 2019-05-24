// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoodComponent.h"
#include "CometCompanion.generated.h"

UCLASS()
class COMET_API ACometCompanion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACometCompanion();

public:
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CompanionMesh;

	UPROPERTY(Category = Collider, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* BrakeShpere;

	UPROPERTY(Category = Collider, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SyncSphere;

	UPROPERTY(Category = Collider, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CameraLockSphere;


	UPROPERTY(Category = Beat, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBeatComponent* BeatComponent;

	bool bIsFree = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMoodEnum MoodType;

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

	UFUNCTION()
	void OnBrakeSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCameraLockSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCameraLockSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION()
	void RespondToBeatPlayed();

	UFUNCTION(BlueprintCallable)
	UNiagaraComponent* SpawnParticle(UNiagaraSystem* SystemTemplate);

	UFUNCTION()
	void RespondToAllBeatsMatched();

public:	
	UFUNCTION(BlueprintCallable)
	void SetCometCompanionFree(AActor* Liberator);

private:
	UFUNCTION()
	void DestoryNS(class UNiagaraComponent* NSToDestroy);
};
