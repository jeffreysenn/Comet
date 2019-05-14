// Fill out your copyright notice in the Description page of Project Settings.


#include "SunCompanion.h"

#include "BeatComponent.h"
#include "NiagaraComponent.h"

void ASunCompanion::BeginPlay()
{
	ACometCompanion::BeginPlay();

	SetBeatAndParticleActive(false);
}

void ASunCompanion::SetBeatAndParticleActive(bool bInActive)
{
	BeatComponent->SetActive(bInActive);
	BeatNiagara->SetActive(bInActive);
}