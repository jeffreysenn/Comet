// Fill out your copyright notice in the Description page of Project Settings.

#include "MoodComponent.h"

// Sets default values for this component's properties
UMoodComponent::UMoodComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Moods.Add(FMoodStruct::FMoodStruct("Red"));
	//Moods.Add(FMoodStruct::FMoodStruct("Green"));
	//Moods.Add(FMoodStruct::FMoodStruct("Blue"));

	MoodMap.Add(EMoodEnum::ME_Type0, false);
	MoodMap.Add(EMoodEnum::ME_Type1, false);
	MoodMap.Add(EMoodEnum::ME_Type2, false);
	MoodMap.Add(EMoodEnum::ME_Type3, false);
}


// Called when the game starts
void UMoodComponent::BeginPlay()
{
	Super::BeginPlay();

	MoodMap[EMoodEnum::ME_Type0] = true;
	
}


// Called every frame
void UMoodComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UMoodComponent::HasMoodType(EMoodEnum MoodType) const
{
	if (MoodMap.Contains(MoodType))
	{
		return MoodMap[MoodType];
	}
	return false;
}

