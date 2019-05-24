// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CometCompanion.h"
#include "SunCompanion.generated.h"

/**
 * 
 */
UCLASS()
class COMET_API ASunCompanion : public ACometCompanion
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	void SetBeatActive(bool bInActive);
	
};
