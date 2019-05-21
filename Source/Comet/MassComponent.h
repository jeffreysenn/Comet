// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MassComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMassCompSignature, float, Mass);

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

	UPROPERTY()
	FMassCompSignature OnMassChanged;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mass")
	float CurrentMass = 0;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintCallable)
	void GainMass(float DeltaMass);

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentMass() const { return CurrentMass; }
		
};
