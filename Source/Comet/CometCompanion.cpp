// Fill out your copyright notice in the Description page of Project Settings.

#include "CometCompanion.h"

#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "CometPawn.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACometCompanion::ACometCompanion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CompanionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CompanionMesh0"));
	RootComponent = CompanionMesh;

	BrakeShpere = CreateDefaultSubobject<USphereComponent>(TEXT("BrakeShpere0"));
	BrakeShpere->SetupAttachment(RootComponent);
	BrakeShpere->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	BrakeShpere->SetSphereRadius(3000.f);
}

// Called when the game starts or when spawned
void ACometCompanion::BeginPlay()
{
	Super::BeginPlay();

	BrakeShpere->OnComponentBeginOverlap.AddDynamic(this, &ACometCompanion::OnBrakeSphereOverlapBegin);
}

// Called every frame
void ACometCompanion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UMoodComponent* ACometCompanion::FindLiberatorMoodComp(AActor* Liberator)
{
	if (Liberator)
	{
		return Liberator->FindComponentByClass<UMoodComponent>();
	}

	return nullptr;
}

void ACometCompanion::OnBrakeSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != NULL)
	{

		ACometPawn* CometPawn = Cast<ACometPawn>(OtherActor);
		if (CometPawn != NULL)
		{
			CometPawn->SetThrustEnabled(false);
			FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(CometPawn->GetActorLocation(), GetActorLocation());
			CometPawn->SetActorRotation(PlayerRot);
		}
	}
}


void ACometCompanion::SetCometCompanionFree(AActor* Liberator)
{
	if (bIsFree) { return; }
	bIsFree = true;
	OnSetFree(Liberator);
}

