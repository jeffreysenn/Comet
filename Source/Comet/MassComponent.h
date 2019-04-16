// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MassComponent.generated.h"


UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class COMET_API UMassComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMassComponent();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mass")
	float MaxMass = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mass")
	float MinMass = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mass")
	float StartMass = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mass")
	float MassLostPerSecond = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mass")
	float DustSpawningMassLoss = 5;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mass")
	float CurrentMass = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mass")
	float LastSpawnMass = 0;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void GainMass(float DeltaMass);

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentMass() const { return CurrentMass; }

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnDust();
		
};
