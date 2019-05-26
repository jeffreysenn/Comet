// Fill out your copyright notice in the Description page of Project Settings.


#include "SunCompanion.h"

#include "BeatComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

ASunCompanion::ASunCompanion()
{
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Static);
	BrakeShpere->SetMobility(EComponentMobility::Static);
	SyncSphere->SetMobility(EComponentMobility::Static);
	CameraLockSphere->SetMobility(EComponentMobility::Static);

	NormalParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NormalParticle0"));
	NormalParticle->SetupAttachment(RootComponent);

	BeatComponent->SetAutoActivate(false);
}

void ASunCompanion::BeginPlay()
{
	ACometCompanion::BeginPlay();

	SyncSphere->OnComponentBeginOverlap.AddDynamic(this, &ASunCompanion::OnSyncSphereBeginOverlap);
	SyncSphere->OnComponentEndOverlap.AddDynamic(this, &ASunCompanion::OnSyncSphereEndOverlap);

	BeatComponent->SetComponentTickEnabled(false);
}

void ASunCompanion::OnSyncSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	BeatComponent->SetComponentTickEnabled(true);
}

void ASunCompanion::OnSyncSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	BeatComponent->SetComponentTickEnabled(false);

}

void ASunCompanion::SetCometCompanionFree(AActor* Liberator)
{
	if (bIsFree) { return; }
	bIsFree = true;

	UMoodComponent* MoodComp = FindLiberatorMoodComp(Liberator);
	if (MoodComp)
	{
		MoodComp->SetMoodTypeBool(MoodType, true);
	}

	BrakeShpere->DestroyComponent();
	SyncSphere->DestroyComponent();
	CameraLockSphere->DestroyComponent();

	OnCompanionSetFree.Broadcast();
	OnSetFree(Liberator);
}
