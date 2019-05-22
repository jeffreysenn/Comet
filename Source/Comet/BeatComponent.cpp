// Fill out your copyright notice in the Description page of Project Settings.

#include "BeatComponent.h"

#include "Components/SphereComponent.h"
#include "CometPawn.h"

UBeatComponent::UBeatComponent(const FObjectInitializer & ObjectInitializer)
{
	bAutoActivate = false;
	PrimaryComponentTick.bCanEverTick = true;
}

void UBeatComponent::BeginPlay()
{
	Super::BeginPlay();

	// Copy UPROPERTY Beats data to BeatsPtr
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
			OnBeatPlayed.Broadcast();

			if (BeatsPtr[CurrentBeatIndex]->BeatSound)
			{
				SetSound(BeatsPtr[CurrentBeatIndex]->BeatSound);
				Play();
			}
			CurrentBeatIndex = (CurrentBeatIndex + 1) % (BeatsPtr.Num());
		}
		// Reset beat matching status after each loop
		if (TimeSinceLastLoopFinished >= LoopLength)
		{
			TimeSinceLastLoopFinished = 0;
			ResetBeatMatch();
		}
	}

	TimeSinceLastLoopFinished += DeltaTime;
}

void UBeatComponent::RequestMatchBeat(ACometPawn* Requester)
{
	RequestedPawn = Requester;

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
			break;
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
