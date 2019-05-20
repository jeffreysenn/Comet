// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoodComponent.generated.h"


UENUM(BlueprintType)
enum class EMoodEnum : uint8
{
	ME_DefaultType,
	ME_Type0, 
	ME_Type1,
	ME_Type2,
	ME_Type3
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMET_API UMoodComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoodComponent();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<EMoodEnum, bool> MoodMap;

protected:


public:	
	UFUNCTION(BlueprintPure)
	bool HasMoodType(EMoodEnum MoodType) const;

	UFUNCTION(BlueprintCallable)
	void SetMoodTypeBool(EMoodEnum MoodType, bool bHasMood);
};
