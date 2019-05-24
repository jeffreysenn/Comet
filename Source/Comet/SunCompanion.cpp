// Fill out your copyright notice in the Description page of Project Settings.


#include "SunCompanion.h"

#include "BeatComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

ASunCompanion::ASunCompanion()
{
	CompanionMesh->SetMobility(EComponentMobility::Static);
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

	SyncSphere->OnComponentBeginOverlap.AddDynamic(this, &ASunCompanion::RespondToOnSyncSphereOverlapBegin);
	SyncSphere->OnComponentEndOverlap.AddDynamic(this, &ASunCompanion::RespondToOnSyncSphereOverlapEnd);
	
}

void ASunCompanion::RespondToOnSyncSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetBeatActive(true);
}

void ASunCompanion::RespondToOnSyncSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SetBeatActive(false);
}

UNiagaraComponent* ASunCompanion::SpawnParticle(UNiagaraSystem* SystemTemplate)
{
	if (SystemTemplate != NULL)
	{
		UNiagaraComponent* BeatParticleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(SystemTemplate, CompanionMesh, TEXT("None"), FVector::ZeroVector, CompanionMesh->GetComponentRotation(), EAttachLocation::KeepRelativeOffset, false);
		BeatParticleComp->SetAbsolute(false, true, false);
		BeatParticleComp->SetWorldRotation(CompanionMesh->GetComponentRotation());
		BeatParticleComp->SetNiagaraVariableLinearColor(TEXT("User.StartColour"), Colour);
		BeatParticleComp->SetNiagaraVariableLinearColor(TEXT("User.EndColour"), FLinearColor(Colour.R, Colour.G, Colour.B, 0));
		BeatParticleComp->SetNiagaraVariableFloat(TEXT("User.SpawnRadius"), SyncSphere->GetScaledSphereRadius());

		BeatParticleComp->OnSystemFinished.AddUniqueDynamic(this, &ACometCompanion::DestoryNS);
		return BeatParticleComp;
	}
	return nullptr;
}

void ASunCompanion::SetBeatActive(bool bInActive)
{
	BeatComponent->SetActive(bInActive);
}