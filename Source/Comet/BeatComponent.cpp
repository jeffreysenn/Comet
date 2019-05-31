// Fill out your copyright notice in the Description page of Project Settings.

#include "BeatComponent.h"

#include "Components/SphereComponent.h"
#include "CometPawn.h"

UBeatComponent::UBeatComponent(const FObjectInitializer & ObjectInitializer)
{
	bAutoActivate = false;
	PrimaryComponentTick.bCanEverTick = true;
}

void UBeatComponent::PlayBeat()
{
	if (Sound != NULL)
	{
		SetVolumeMultiplier(VolumnMultiplier);
		Play();
	}
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

bool UBeatComponent::RequestMatchBeat(ACometPawn* Requester)
{
	RequestedPawn = Requester;

	bool bMatched = false;

	for (int32 BeatIndex = 0; BeatIndex < BeatsPtr.Num(); BeatIndex++)
	{
		if (BeatsPtr.IsValidIndex(BeatIndex))
		{
			if (TimeSinceLastLoopFinished >= BeatsPtr[BeatIndex]->TimePoint - BeatsPtr[BeatIndex]->TimeWindowHalfLength
				&& TimeSinceLastLoopFinished <= BeatsPtr[BeatIndex]->TimePoint + BeatsPtr[BeatIndex]->TimeWindowHalfLength)
			{
				if (!BeatsPtr[BeatIndex]->bBeatMatched)
				{
					BeatsPtr[BeatIndex]->bBeatMatched = true;
					bMatched = true;
					break;
				}
			}
		}
	}


	// Anti spamming code
	if (!bMatched)
	{
		ResetBeatMatch();
		return bMatched;
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

	return bMatched;
}

void UBeatComponent::ResetBeatMatch()
{
	for (auto* BeatPtr : BeatsPtr)
	{
		BeatPtr->bBeatMatched = false;

	}
}
