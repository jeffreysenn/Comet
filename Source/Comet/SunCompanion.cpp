// Fill out your copyright notice in the Description page of Project Settings.


#include "SunCompanion.h"

#include "BeatComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

ASunCompanion::ASunCompanion()
{
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Static);
	BrakeShpere->SetMobility(EComponentMobility::Static);
	SyncSphere->SetMobility(EComponentMobility::Static);
	CameraLockSphere->SetMobility(EComponentMobility::Static);

	NormalParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NormalParticle0"));
	NormalParticle->SetupAttachment(RootComponent);

	BeatComponent->SetAutoActivate(false);

	Colours =
	{
		FSunCompanionColourStruct(1, 4, 1, FLinearColor(0.501042,0.486314, 0.470609, 0.494000), FLinearColor(1.000000,0.984314,0.901961,1.000000), 3),
		FSunCompanionColourStruct(0.3f, 10.f, 2.f, FLinearColor(1.000000,0.883333,0.300000,1.000000), FLinearColor(1.000000,0.968000,0.800000,1.000000), 10),
		FSunCompanionColourStruct(0.1f, 15.f, 4, FLinearColor(1.000000,0.866667,0.200000,1.000000), FLinearColor(1.000000,0.950000,0.700000,1.000000), 15), 
		FSunCompanionColourStruct(0.f, 20.f, 8.f, FLinearColor(1.000000,0.833333,0.000000,1.000000), FLinearColor(1.000000,0.936001,0.600000,1.000000), 20),
		FSunCompanionColourStruct(-1.f, 30, 30, FLinearColor(1.000000,0.666667,0.000000,1.000000), FLinearColor(1.000000,0.916667,0.500000,1.000000), 30)
	};
}


void ASunCompanion::BeginPlay()
{
	ACometCompanion::BeginPlay();

	SyncSphere->OnComponentBeginOverlap.AddDynamic(this, &ASunCompanion::OnSyncSphereBeginOverlap);
	SyncSphere->OnComponentEndOverlap.AddDynamic(this, &ASunCompanion::OnSyncSphereEndOverlap);

	BeatComponent->SetComponentTickEnabled(false);

	TArray<ACometCompanion*> OtherCompanions = FindAllOtherCompanions();
	for (ACometCompanion* Companion : OtherCompanions)
	{
		if (Companion != NULL)
		{
			Companion->OnCompanionSetFree.AddUniqueDynamic(this, &ASunCompanion::RespondToOnCompanionSetFree);
		}
	}

	if (Colours.IsValidIndex(ColourIndex))
	{
		Colour = Colours[ColourIndex].ParticleColour;
	}
}

void ASunCompanion::OnSyncSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	BeatComponent->SetComponentTickEnabled(true);
}

void ASunCompanion::OnSyncSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	BeatComponent->SetComponentTickEnabled(false);

}

void ASunCompanion::SetCometCompanionFree(AActor* Liberator)
{
	if (bIsFree) { return; }
	bIsFree = true;

	BrakeShpere->DestroyComponent();
	SyncSphere->DestroyComponent();
	CameraLockSphere->DestroyComponent();

	OnCompanionSetFree.Broadcast();
	OnSetFree(Liberator);
}

TArray<class ACometCompanion*> ASunCompanion::FindAllOtherCompanions()
{
	TArray<AActor*> OutFoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACometCompanion::StaticClass(), OutFoundActors);
	TArray<ACometCompanion*> CompanionArr;
	for (AActor* FoundActor: OutFoundActors)
	{
		if (!FoundActor->IsA(ASunCompanion::StaticClass()))
		{
			ACometCompanion* FoundCompanion = Cast<ACometCompanion>(FoundActor);
			if (FoundCompanion != NULL)
			{
				CompanionArr.Add(FoundCompanion);
			}
		}
	}
	return CompanionArr;
}

void ASunCompanion::RespondToOnCompanionSetFree()
{
	// UE_LOG(LogTemp, Warning, TEXT("A companion was set free!"));

	ColourIndex++;

	if (Colours.IsValidIndex(ColourIndex) && Colours.IsValidIndex(ColourIndex-1))
	{
		UpdateColour(Colours[ColourIndex - 1], Colours[ColourIndex]);
	}
}
