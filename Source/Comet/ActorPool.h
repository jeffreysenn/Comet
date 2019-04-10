// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorPool.generated.h"

UCLASS()
class COMET_API AActorPool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorPool();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool")
	TSubclassOf<AActor> ActorObj;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool")
	int32 PoolSize = 1000;

private:
	TArray<AActor*> ActorsInPool;

	TArray<AActor*> ActivatedActors;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> InitializePoolAtSize(int32 Size);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ReturnToPool(AActor* ActorToAdd);

	UFUNCTION(BlueprintCallable)
	AActor* SpawnFromPool(FTransform Transform);

	void SetActorActive(AActor* InActor, bool bShouldActivate);
};
