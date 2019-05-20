// Fill out your copyright notice in the Description page of Project Settings.


#include "Crystal.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/MaterialBillboardComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ACrystal::ACrystal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CrystalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrystalMesh0"));
	RootComponent = CrystalMesh;
	CrystalMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	CrystalMesh->SetMobility(EComponentMobility::Static);

	AttractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttractionSphere0"));
	AttractionSphere->SetupAttachment(RootComponent);
	AttractionSphere->InitSphereRadius(1300.f);
	AttractionSphere->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	AttractionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACrystal::OnAttractionSphereOverlapBegin);
	AttractionSphere->SetMobility(EComponentMobility::Static);

	GlowBillboard = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("GlowBillboard0"));
	GlowBillboard->SetupAttachment(RootComponent);

}



void ACrystal::OnAttractionSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetActorEnableCollision(false);
	CrystalMesh->SetVisibility(false);

	Obtainer = OtherActor;

	ShinePlease();


}
