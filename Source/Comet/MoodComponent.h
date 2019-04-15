// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoodComponent.generated.h"


UENUM(BlueprintType)
enum class EMoodEnum : uint8
{
	ME_Type0, 
	ME_Type1,
	ME_Type2,
	ME_Type3
};

USTRUCT(BlueprintType)
struct FMoodStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Threshold;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Count = 0;

	FMoodStruct()
	{

	}

	FMoodStruct(FName InName, int32 InThreshold = 10)
	{
		Name = InName;
		Threshold = InThreshold;
		Count = 0;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMET_API UMoodComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoodComponent();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMoodStruct> Moods;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<EMoodEnum, bool> MoodMap;

protected:
	UPROPERTY(VisibleAnywhere)
	FName CurrentMood;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
	//UFUNCTION(BlueprintCallable)
	//void ChangeMood(FName InMoodName);

	UFUNCTION(BlueprintPure)
	FORCEINLINE FName GetCurrentMood() const { return CurrentMood; } 

	UFUNCTION(BlueprintPure)
	bool HasMoodType(EMoodEnum MoodType) const;
};
