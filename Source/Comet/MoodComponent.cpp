// Fill out your copyright notice in the Description page of Project Settings.

#include "MoodComponent.h"

// Sets default values for this component's properties
UMoodComponent::UMoodComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MoodMap.Add(EMoodEnum::ME_DefaultType, true);
	MoodMap.Add(EMoodEnum::ME_Type0, false);
	MoodMap.Add(EMoodEnum::ME_Type1, false);
	MoodMap.Add(EMoodEnum::ME_Type2, false);
	MoodMap.Add(EMoodEnum::ME_Type3, false);
}


bool UMoodComponent::HasMoodType(EMoodEnum MoodType) const
{
	if (MoodMap.Contains(MoodType))
	{
		return MoodMap[MoodType];
	}
	return false;
}

void UMoodComponent::SetMoodTypeBool(EMoodEnum MoodType, bool bHasMood = true)
{
	if (MoodMap.Contains(MoodType))
	{
		MoodMap[MoodType] = bHasMood;
	}
}

