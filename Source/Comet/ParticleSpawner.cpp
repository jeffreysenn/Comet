// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleSpawner.h"

// Sets default values for this component's properties
UParticleSpawner::UParticleSpawner()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UParticleSpawner::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		//getting an amount of particles to spawn and spawning them in an area around the BP
		for (int i = 0; i < Amount; i++)
		{
			//SpawnDust = (UClass*)SpawnDust.Object->GeneratedClass;
		}
		//
	}
	
}


// Called every frame
void UParticleSpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int32 UParticleSpawner::GetAmount()
{
	return Amount;
}

