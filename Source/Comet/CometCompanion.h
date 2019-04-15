// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CometCompanion.generated.h"

UCLASS()
class COMET_API ACometCompanion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACometCompanion();

public:
	bool bIsFree = false;

//private:
//	class ACometPawn* CometPawn;
//	class UMoodComponent* MoodComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnSetFree(AActor* Liberator);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetCometCompanionFree(AActor* Liberator);
};
