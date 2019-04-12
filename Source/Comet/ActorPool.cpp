// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool.h"

// Sets default values
AActorPool::AActorPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActorPool::BeginPlay()
{
	Super::BeginPlay();
}

TArray<AActor*> AActorPool::InitializePoolAtSize(int32 Size)
{
	auto World = GetWorld();
	if (World)
	{
		for (int32 i = 0; i < Size; i++)
		{
			auto SpawnedActor = World->SpawnActor<AActor>(ActorObj, FVector::ZeroVector, FRotator::ZeroRotator);
			ActorsInPool.Add(SpawnedActor);
		}

	}
	return ActorsInPool;
}

// Called every frame
void AActorPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActorPool::ReturnToPool(AActor* ActorToReturn)
{
	if (ActorToReturn) 
	{
		// When pool is not full, put to pool
		if (ActorsInPool.Num() <= PoolSize)
		{
			DeactivateActor(ActorToReturn);
			ActorsInPool.Add(ActorToReturn);
			ActivatedActors.Remove(ActorToReturn);
		}
		// Destroy when pool is full
		else
		{
			ActorToReturn->Destroy();
		}
	
	}
}

AActor* AActorPool::SpawnFromPool(FTransform Transform)
{
	// when pool is empty, return the first activated actor to the pool and call spawn again
	if (ActorsInPool.Num() <= 0) 
	{
		if (ActivatedActors.IsValidIndex(0) && ActivatedActors[0])
		{
			ReturnToPool(ActivatedActors[0]);
		}
	}
	AActor* ActorToSpawn = ActorsInPool.Last();
	if (ActorToSpawn)
	{
		ActorToSpawn->SetActorTransform(Transform);
		ActivateActor(ActorToSpawn);
		ActorsInPool.Pop();
		ActivatedActors.Add(ActorToSpawn);
	}
	return ActorToSpawn;
}