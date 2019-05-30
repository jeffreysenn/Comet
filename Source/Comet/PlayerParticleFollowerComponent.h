// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerParticleFollowerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMET_API UPlayerParticleFollowerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerParticleFollowerComponent();

	//This is the actor that will be spawned around the player, in this case it will be used for particle system actors.
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> ActorToSpawn;

	//Used for determening how close the player needs to be before an actor spawns.
	UPROPERTY(EditAnywhere)
		float SpawnDistance = 2000.0f;
	
	//Used for determening how far away the player needs to be before a spawned actor is destroyed.
	UPROPERTY(EditAnywhere)
		float DestroyDistance = 4000.0f;

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	//Based of the currently spawned particle system that the player is in, always needs to be 27 positions.
	TArray<FVector> SpawnPositionsInworld;

	//the spawned actor in the world, used for determening wheter to spawn new particle systems or not, as well as destroying old actors in world.
	TArray<AActor*> ActorsInWorld;

	void CalculateSpawnPositions(AActor* ActorThatThePlayerIsIn);

	float SquareByX(float ValueToSquare, int32 X);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
