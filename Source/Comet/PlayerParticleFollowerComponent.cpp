// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerParticleFollowerComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"

// Sets default values for this component's properties
UPlayerParticleFollowerComponent::UPlayerParticleFollowerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerParticleFollowerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		if (ActorToSpawn)
		{
			
			ActorsInWorld.Add( GetWorld()->SpawnActor<AActor>(ActorToSpawn, GetOwner()->GetActorLocation(), FRotator(0, 0, 0)));
	
			
			
			CalculateSpawnPositions(ActorsInWorld[0]);

			for (int32 i = 1; i < SpawnPositionsInworld.Num(); i++)
			{
				ActorsInWorld.Add(GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnPositionsInworld[i], FRotator(0, 0, 0)));
			}
			
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerParticleFollowerComponent @BeginPlay no actor to spawn is set"));
			DestroyComponent();
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerParticleFollowerComponent @BeginPlay could not get owner"));
		DestroyComponent();
		return;
	}
	// ...
	
}

//Calculates a cube of cordinates to use for spawning ActorToSpawn based on its size, Due Note that size 10 seems to be optimal
void UPlayerParticleFollowerComponent::CalculateSpawnPositions(AActor * ActorThatThePlayerIsIn)
{
	if (ActorThatThePlayerIsIn)
	{
		//SpawnPositionsInworld.Add(SpawnPositionsInworld[0]);
		SpawnPositionsInworld.Empty();
		SpawnPositionsInworld.Add(ActorThatThePlayerIsIn->GetActorLocation());


		//FVector TEMP = FVector(ActorThatThePlayerIsIn->GetActorLocation().X + SquareByX(ActorThatThePlayerIsIn->GetActorScale().X, 3), 0, 0);
		//FVector TEMP = FVector(ActorThatThePlayerIsIn->GetActorLocation().X + SquareByX(ActorThatThePlayerIsIn->GetActorScale.X, 3), ActorThatThePlayerIsIn->GetActorLocation().Y, ActorThatThePlayerIsIn->GetActorLocation().Z);
		//SpawnPositionsInworld.Add(TEMP);
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + (ActorThatThePlayerIsIn->GetActorScale().X*100), SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - (ActorThatThePlayerIsIn->GetActorScale().X*100), SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y + (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y - (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z + (ActorThatThePlayerIsIn->GetActorScale().X * 100)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z - (ActorThatThePlayerIsIn->GetActorScale().X * 100)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Y + (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Y - (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Y + (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Y - (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z + (ActorThatThePlayerIsIn->GetActorScale().X * 100)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z - (ActorThatThePlayerIsIn->GetActorScale().X * 100)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z - (ActorThatThePlayerIsIn->GetActorScale().X * 100)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z + (ActorThatThePlayerIsIn->GetActorScale().X * 100)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y + (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Z + (ActorThatThePlayerIsIn->GetActorScale().X * 100)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y - (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Z - (ActorThatThePlayerIsIn->GetActorScale().X * 100)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y + (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Z - (ActorThatThePlayerIsIn->GetActorScale().X * 100)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y - (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Z + (ActorThatThePlayerIsIn->GetActorScale().X * 100)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Y + (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Z + (ActorThatThePlayerIsIn->GetActorScale().X * 100)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Y - (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Z - (ActorThatThePlayerIsIn->GetActorScale().X * 100)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Y + (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Z - (ActorThatThePlayerIsIn->GetActorScale().X * 100)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Y - (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Z + (ActorThatThePlayerIsIn->GetActorScale().X * 100)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Y + (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Z + (ActorThatThePlayerIsIn->GetActorScale().X * 100)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Y - (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Z - (ActorThatThePlayerIsIn->GetActorScale().X * 100)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Y - (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Z + (ActorThatThePlayerIsIn->GetActorScale().X * 100)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Y + (ActorThatThePlayerIsIn->GetActorScale().X * 100), SpawnPositionsInworld[0].Z - (ActorThatThePlayerIsIn->GetActorScale().X * 100)));


		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) *2), SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) *2), SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Y + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Y - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Y + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Y - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Z + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Z - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Z - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Z + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Y + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Z + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Y - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Z - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Y + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Z - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Y - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Z + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Y + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Z + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Y - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Z - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Y - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Z + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Y + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2), SpawnPositionsInworld[0].Z - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 2)));

		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Y + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Y - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Y + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Y - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Z));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Y, SpawnPositionsInworld[0].Z + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Z + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Z - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Z - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X, SpawnPositionsInworld[0].Y - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Z + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Y + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Z + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Y - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Z - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Y + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Z - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Y - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Z + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Y + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Z + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Y - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Z - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Y - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Z + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3)));
		SpawnPositionsInworld.Add(FVector(SpawnPositionsInworld[0].X - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Y + ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3), SpawnPositionsInworld[0].Z - ((ActorThatThePlayerIsIn->GetActorScale().X * 100) * 3)));
	}
	else
	{
		return;
	}
	FMath::Pow(ActorThatThePlayerIsIn->GetActorScale().X, 3);
}

float UPlayerParticleFollowerComponent::SquareByX(float ValueToSquare, int32 X)
{
	float ReturnValue = ValueToSquare;
	for (int32 i = 0; i < X; i++)
	{
		ReturnValue *= ValueToSquare;
	}
	return ReturnValue;
}

// Called every frame
void UPlayerParticleFollowerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (GetOwner()) 
	{
		if (ActorsInWorld.Num())
		{
			if (SpawnPositionsInworld.Num())
			{
				for (int32 i = 0; i < SpawnPositionsInworld.Num(); i++)
				{
					int32 count = 0;

					if (SpawnPositionsInworld.IsValidIndex(i))
					{
						for (int32 g = 0; g < ActorsInWorld.Num(); g++)
						{
							if (ActorsInWorld[g] && ActorsInWorld.IsValidIndex(g))
							{
								if (FVector::Dist(SpawnPositionsInworld[i], ActorsInWorld[g]->GetActorLocation()) < 1000.0f)
								{
									count++;
								}
							}


						}

					}
					else
					{

					}

					if (count == 0)
					{
						ActorsInWorld.Add(GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnPositionsInworld[i], FRotator(0, 0, 0)));
					}

				}

				

			
				for (int32 g = 0; g < ActorsInWorld.Num(); g++)
				{ 
					
					if (ActorsInWorld[g] && ActorsInWorld.IsValidIndex(g))
					{
						if (FVector::Dist(ActorsInWorld[g]->GetActorLocation(), GetOwner()->GetActorLocation()) < SpawnDistance)
						{
							CalculateSpawnPositions(ActorsInWorld[g]);

								
						}
					
						

						if (FVector::Dist(ActorsInWorld[g]->GetActorLocation(), GetOwner()->GetActorLocation()) > DestroyDistance)
						{
							ActorsInWorld[g]->Destroy();
							ActorsInWorld.RemoveAt(g);
						}

					}
					else
					{

					}
						


				}

				
			
				


			}
			else
			{
				return;
			}
		}
		else
		{
			ActorsInWorld.Empty();
			ActorsInWorld.Add(GetWorld()->SpawnActor<AActor>(ActorToSpawn, GetOwner()->GetActorLocation(), FRotator(0, 0, 0)));



			CalculateSpawnPositions(ActorsInWorld[0]);

			for (int32 i = 1; i < SpawnPositionsInworld.Num(); i++)
			{
				ActorsInWorld.Add(GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnPositionsInworld[i], FRotator(0, 0, 0)));
			}
		}
	}
	else
	{
		return;
	}
	
	
	// ...
}

