// Fill out your copyright notice in the Description page of Project Settings.


#include "SunCompanion.h"

#include "BeatComponent.h"

void ASunCompanion::BeginPlay()
{
	ACometCompanion::BeginPlay();

	SetBeatActive(false);
}

void ASunCompanion::SetBeatActive(bool bInActive)
{
	BeatComponent->SetActive(bInActive);
}