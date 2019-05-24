// Fill out your copyright notice in the Description page of Project Settings.


#include "Crystal.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/MaterialBillboardComponent.h"
#include "MassComponent.h"
#include "CometPawn.h"
#include "Kismet/KismetMathLibrary.h"



// Sets default values
ACrystal::ACrystal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AttractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttractionSphere0"));
	AttractionSphere->SetupAttachment(RootComponent);
	AttractionSphere->SetAbsolute(false, true, true);
	AttractionSphere->InitSphereRadius(4000.f);
	AttractionSphere->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	AttractionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACrystal::OnAttractionSphereOverlapBegin);
	AttractionSphere->SetMobility(EComponentMobility::Static);

	GlowBillboard = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("GlowBillboard0"));
	GlowBillboard->SetupAttachment(RootComponent);
	GlowBillboard->SetAbsolute(false, true, true);
}

//void ACrystal::OnConstruction(const FTransform& Transform)
//{
//	Super::OnConstruction(Transform);
//
//	// Set random mesh at scale and rotation
//	int32 MeshIndex = FMath::RandRange(0, MeshArr.Num() - 1);
//	if (MeshArr.IsValidIndex(MeshIndex) && MeshArr[MeshIndex] != NULL)
//	{
//		GetStaticMeshComponent()->SetStaticMesh(MeshArr[MeshIndex]);
//
//		FVector RandomScale = GetRandomScale(MinMaxScale.X, MinMaxScale.Y);
//		GetStaticMeshComponent()->SetWorldScale3D(RandomScale);
//
//		FRotator RandomRotator = UKismetMathLibrary::RandomRotator(true);
//		GetStaticMeshComponent()->SetWorldRotation(RandomRotator);
//	}
//}

void ACrystal::OnAttractionSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	Obtainer = OtherActor;

	if (Obtainer != NULL)
	{
		UMoodComponent* MoodComp = Obtainer->FindComponentByClass<UMoodComponent>();
		if (MoodComp != NULL)
		{
			if (MoodComp->HasMoodType(MoodType))
			{
				GetStaticMeshComponent()->SetVisibility(false);
				SetActorEnableCollision(false);
				ACometPawn* CometPawn = Cast<ACometPawn>(Obtainer);
				if (CometPawn != NULL)
				{
					CometPawn->RequestDash(DashCharge);
				}

				ShinePlease();
			}
		}
	}


}

FVector ACrystal::GetRandomScale(float MinScale, float MaxScale)
{
	float ScaleOneDimen = FMath::RandRange(MinScale, MaxScale);
	return FVector(1,1,1) * ScaleOneDimen;
}
