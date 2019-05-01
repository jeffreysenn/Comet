// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "BeatComponent.generated.h"


USTRUCT(BlueprintType)
struct FBeatStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float TimePoint;

	UPROPERTY(EditAnywhere)
	float TimeWindowHalfLength = 0.2f;

	UPROPERTY(EditAnywhere)
	USoundBase* BeatSound;

	UPROPERTY(VisibleAnywhere)
	bool bBeatMatched = false;

	FBeatStruct()
	{
	}
};



/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBeatDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COMET_API UBeatComponent : public UAudioComponent
{
	GENERATED_BODY()
	
public:
	UBeatComponent( const FObjectInitializer &ObjectInitializer );

public:
	UPROPERTY(BlueprintAssignable)
	FBeatDelegate OnAllBeatsMatched;

	UPROPERTY(BlueprintAssignable)
	FBeatDelegate OnBeatPlayed;


protected:
	/** The time points of the beat */
	UPROPERTY(EditAnywhere, Category = Beat)
	TArray<FBeatStruct> Beats;

	/** The length of the whole beat loop */
	UPROPERTY(EditAnywhere, Category = Beat)
	float LoopLength;

private:
	TArray<FBeatStruct*> BeatsPtr;

	float TimeSinceLastLoopFinished = 0;

	int32 CurrentBeatIndex = 0;


public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void RequestMatchBeat();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;



private:
	void ResetBeatMatch();

};
