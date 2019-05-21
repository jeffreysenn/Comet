// Fill out your copyright notice in the Description page of Project Settings.

#include "MassComponent.h"

// Sets default values for this component's properties
UMassComponent::UMassComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UMassComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentMass = StartMass;
}

void UMassComponent::GainMass(float DeltaMass)
{
	CurrentMass = FMath::Clamp(CurrentMass + DeltaMass, MinMass, MaxMass);
	OnMassChanged.Broadcast(CurrentMass);
}

