// Fill out your copyright notice in the Description page of Project Settings.

#include "CometCompanion.h"

#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "BeatComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "MoodComponent.h"
#include "Components/MaterialBillboardComponent.h"
#include "CometPawn.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ACometCompanion::ACometCompanion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);

	BrakeShpere = CreateDefaultSubobject<USphereComponent>(TEXT("BrakeShpere0"));
	BrakeShpere->SetupAttachment(RootComponent);
	BrakeShpere->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	BrakeShpere->SetSphereRadius(3000.f);

	SyncSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SyncShpere0"));
	SyncSphere->SetupAttachment(RootComponent);
	SyncSphere->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	SyncSphere->SetSphereRadius(15000.f);

	CameraLockSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CameraLockSphere0"));
	CameraLockSphere->SetupAttachment(RootComponent);
	CameraLockSphere->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	CameraLockSphere->SetSphereRadius(4000.f);

	BeatComponent = CreateDefaultSubobject<UBeatComponent>(TEXT("BeatComp0"));
	BeatComponent->SetupAttachment(RootComponent);

	MaterialBillboard = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("MaterialBillboard0"));
	MaterialBillboard->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACometCompanion::BeginPlay()
{
	Super::BeginPlay();

	BrakeShpere->OnComponentBeginOverlap.AddDynamic(this, &ACometCompanion::OnBrakeSphereOverlapBegin);

	CameraLockSphere->OnComponentBeginOverlap.AddDynamic(this, &ACometCompanion::OnCameraLockSphereOverlapBegin);
	CameraLockSphere->OnComponentEndOverlap.AddDynamic(this, &ACometCompanion::OnCameraLockSphereOverlapEnd);

	BeatComponent->OnBeatPlayed.AddUniqueDynamic(this, &ACometCompanion::RespondToBeatPlayed);
	BeatComponent->OnAllBeatsMatched.AddUniqueDynamic(this, &ACometCompanion::RespondToAllBeatsMatched);
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
		}
	}
}

void ACometCompanion::OnCameraLockSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor != NULL)
	{
		ACometPawn* CometPawn = Cast<ACometPawn>(OtherActor);
		if (CometPawn != NULL)
		{
			CometPawn->RequestLockOnActor(this, true);
		}
	}

}

void ACometCompanion::OnCameraLockSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != NULL)
	{
		ACometPawn* CometPawn = Cast<ACometPawn>(OtherActor);
		if (CometPawn != NULL)
		{
			CometPawn->RequestLockOnActor(NULL, false);
		}
	}
}

void ACometCompanion::RespondToBeatPlayed()
{
	SpawnParticle(BeatParticleTemplate);
}

UNiagaraComponent* ACometCompanion::SpawnParticle(UNiagaraSystem* SystemTemplate)
{
	if (SystemTemplate != NULL)
	{
		UNiagaraComponent* BeatParticleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(SystemTemplate, GetStaticMeshComponent(), TEXT("None"), FVector::ZeroVector, GetStaticMeshComponent()->GetComponentRotation(), EAttachLocation::KeepRelativeOffset, false);
		BeatParticleComp->SetAbsolute(false, true, false);
		BeatParticleComp->SetWorldRotation(GetStaticMeshComponent()->GetComponentRotation());
		BeatParticleComp->SetNiagaraVariableLinearColor(TEXT("User.StartColour"), Colour);
		BeatParticleComp->SetNiagaraVariableLinearColor(TEXT("User.EndColour"), FLinearColor(Colour.R, Colour.G, Colour.B, 0));

		BeatParticleComp->OnSystemFinished.AddUniqueDynamic(this, &ACometCompanion::DestoryNS);
		return BeatParticleComp;
	}
	return NULL;
}

void ACometCompanion::RespondToAllBeatsMatched()
{
	if (BeatComponent != NULL)
	{
		SetCometCompanionFree(BeatComponent->GetRequestedPawn());
	}

}


void ACometCompanion::SetCometCompanionFree(AActor* Liberator)
{
	if (bIsFree) { return; }
	bIsFree = true;

	UMoodComponent* MoodComp = FindLiberatorMoodComp(Liberator);
	if (MoodComp)
	{
		MoodComp->SetMoodTypeBool(MoodType, true);
	}

	BrakeShpere->DestroyComponent();
	SyncSphere->DestroyComponent();
	CameraLockSphere->DestroyComponent();
	SetActorEnableCollision(false);

	OnCompanionSetFree.Broadcast();
	OnSetFree(Liberator);
}

void ACometCompanion::DestoryNS(UNiagaraComponent* NSToDestroy)
{
	if (!NSToDestroy) return;
	if (!NSToDestroy->IsValidLowLevel()) return;
	NSToDestroy->ConditionalBeginDestroy(); //instantly clears UObject out of memory
}

