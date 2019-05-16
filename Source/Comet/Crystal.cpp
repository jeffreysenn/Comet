// Fill out your copyright notice in the Description page of Project Settings.


#include "Crystal.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"

// Sets default values
ACrystal::ACrystal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CrystalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrystalMesh0"));
	RootComponent = CrystalMesh;

	AttractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttractionSphere0"));
	AttractionSphere->SetupAttachment(RootComponent);
	AttractionSphere->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	TrailParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailParticle0"));
	TrailParticle->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACrystal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACrystal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

