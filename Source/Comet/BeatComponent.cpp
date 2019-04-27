// Fill out your copyright notice in the Description page of Project Settings.

#include "BeatComponent.h"

UBeatComponent::UBeatComponent(const FObjectInitializer & ObjectInitializer)
{
	bAutoActivate = false;
	PrimaryComponentTick.bCanEverTick = true;
}

void UBeatComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto Beat : Beats)
	{
		FBeatStruct* BeatPtr = new FBeatStruct();
		*BeatPtr = Beat;
		BeatsPtr.Add(BeatPtr);
	}

	TimeSinceLastLoopFinished = 0;
	
}

void UBeatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (auto* BeatPtr : BeatsPtr)
	{
		if (BeatPtr)
		{
			delete BeatPtr;
		}
	}
}

void UBeatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	if (BeatsPtr.IsValidIndex(CurrentBeatIndex))
	{
		if (TimeSinceLastLoopFinished <= BeatsPtr[CurrentBeatIndex]->TimePoint && TimeSinceLastLoopFinished + DeltaTime > BeatsPtr[CurrentBeatIndex]->TimePoint)
		{
			if (Sound)
			{
				Play();
			}
			CurrentBeatIndex = (CurrentBeatIndex + 1) % (Beats.Num());
		}
		if (TimeSinceLastLoopFinished >= LoopLength)
		{
			TimeSinceLastLoopFinished = 0;
			ResetBeatMatch();
		}
	}

	TimeSinceLastLoopFinished += DeltaTime;
}

void UBeatComponent::RequestMatchBeat()
{
	if (BeatsPtr.IsValidIndex(CurrentBeatIndex))
	{
		if (TimeSinceLastLoopFinished >= BeatsPtr[CurrentBeatIndex]->TimePoint - BeatsPtr[CurrentBeatIndex]->TimeWindowHalfLength
			&& TimeSinceLastLoopFinished <= BeatsPtr[CurrentBeatIndex]->TimePoint + BeatsPtr[CurrentBeatIndex]->TimeWindowHalfLength)
		{
			BeatsPtr[CurrentBeatIndex]->bBeatMatched = true;
		}

	}

	bool bAllBeatsMatched = true;
	for (auto* BeatPtr : BeatsPtr)
	{
		if (!BeatPtr -> bBeatMatched)
		{
			bAllBeatsMatched = false;
		}
	}
	if (bAllBeatsMatched)
	{
		OnAllBeatsMatched.Broadcast();
	}
}

void UBeatComponent::ResetBeatMatch()
{
	for (auto* BeatPtr : BeatsPtr)
	{
		BeatPtr->bBeatMatched = false;

	}
}
