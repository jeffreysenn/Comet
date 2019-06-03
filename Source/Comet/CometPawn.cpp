// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "CometPawn.h"

#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "CometPlayerController.h"
#include "CometCompanion.h"
#include "BeatComponent.h"
#include "MoodComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "CometCompanion.h"
#include "SunCompanion.h"
#include "MassComponent.h"
#include "Kismet/KismetMathLibrary.h"


ACometPawn::ACometPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	RootCollider = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollider0"));
	RootCollider->InitSphereRadius(100.f);
	RootCollider->SetNotifyRigidBodyCollision(true);
	RootComponent = RootCollider;

	// Create static mesh component
	CometMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CometMesh0"));
	CometMesh->SetupAttachment(RootComponent);

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);	// Attach SpringArm to RootComponent

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);	// Attach the camera
	Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller

	BeatAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("BeatAudio0"));
	BeatAudio->SetupAttachment(RootComponent);

	MassComp = CreateDefaultSubobject<UMassComponent>(TEXT("MassComp0"));
	MassComp->OnMassChanged.AddDynamic(this, &ACometPawn::CheckChangeMesh);

	MoodComp = CreateDefaultSubobject<UMoodComponent>(TEXT("MoodComp0"));

	// Set handling parameters
	OriginalPitch = PitchSpeed;
	OriginalYaw = YawSpeed;
}

void ACometPawn::BeginPlay()
{
	Super::BeginPlay();

	CurrentMeshIndex = 0;
	ChangeMesh(CurrentMeshIndex);

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASunCompanion::StaticClass(), OutActors);
	if (OutActors.IsValidIndex(0))
	{
		SunActor = OutActors[0];
	}
}

void ACometPawn::Tick(float DeltaSeconds)
{
	// Call any parent class Tick implementation
	Super::Tick(DeltaSeconds);

	DashInput();

	const FVector DashMove = FVector(CurrentDashSpeed * DeltaSeconds, 0.f, 0.f);
	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaSeconds, 0.f, 0.f) + DashMove;

	// Move forwards (with sweep so we stop when we collide with things)
	AddActorLocalOffset(LocalMove, true);

	// Calculate change in rotation this frame
	FRotator DeltaRotation(0,0,0);
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaSeconds;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaSeconds;
	DeltaRotation.Roll = CurrentRollSpeed * DeltaSeconds;

	// Stop abs pitch increase if greater than max pitch angle
	if (FMath::Abs(GetActorRotation().Pitch) > MaxPitchAngle && FMath::Abs(GetActorRotation().Pitch + DeltaRotation.Pitch) > FMath::Abs(GetActorRotation().Pitch))
	{
		DeltaRotation.Pitch = 0;
	}

	// Rotate root
	AddActorLocalRotation(DeltaRotation);

	// Roll the comet mesh
	float CurrentSpeed = CurrentForwardSpeed + CurrentDashSpeed;
	// UE_LOG(LogTemp, Warning, TEXT("CurrentSpeed: %f"), CurrentSpeed);
	RollForward(CometMesh, CurrentSpeed * RollMod);

	// Lock camera on target actor
	if (bShouldCamLockOnActor)
	{
		LockCameraOnActor(ActorToLockCam);
	}

}

void ACometPawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	float HitTime = GetWorld()->GetTimeSeconds();

	if (HitTime - LastCollisionTime > MinCollisionInterval)
	{
		// Deflect along the surface when we collide.
		FRotator CurrentRotation = GetActorRotation();
		SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.5f));
		CurrentForwardSpeed *= CollisionSpeedCoefficient;

		if (CollisionSound != NULL)
		{
			UGameplayStatics::SpawnSoundAtLocation(this, CollisionSound, HitLocation);
		}

		if (CollisionNiagaraActor != NULL)
		{
			AActor* NiagaraActor = GetWorld()->SpawnActor<AActor>(CollisionNiagaraActor, HitLocation, UKismetMathLibrary::MakeRotFromZ(HitNormal));
			const FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, true);
			NiagaraActor->AttachToComponent(OtherComp, Rules);
		}

	}
	else
	{
		CurrentForwardSpeed = 0;
	}

	LastCollisionTime = HitTime;
	

}

void ACometPawn::RequestDash(float DeltaCharge)
{
	DashCharged = FMath::Min(MaxDashCharge, DashCharged + DeltaCharge);
}

void ACometPawn::RequestLockOnActor(AActor* ActorToLock, bool bShouldLock)
{
	if (ActorToLock == NULL || !bShouldLock)
	{
		bShouldCamLockOnActor = false;
		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	}
	else if(bShouldLock && !bShouldCamLockOnActor)
	{
		bShouldCamLockOnActor = bShouldLock;
	}

	ActorToLockCam = ActorToLock;
}




void ACometPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // Check if PlayerInputComponent is valid (not NULL)
	check(PlayerInputComponent);

	// Bind our control axis' to callback functions
	PlayerInputComponent->BindAxis("Thrust", this, &ACometPawn::ThrustInput);
	PlayerInputComponent->BindAxis("MoveUp", this, &ACometPawn::MoveUpInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACometPawn::MoveRightInput);


	PlayerInputComponent->BindAxis("MoveUp_Stick", this, &ACometPawn::MoveUpInput_Stick);
	PlayerInputComponent->BindAxis("MoveRight_Stick", this, &ACometPawn::MoveRightInput_Stick);

	PlayerInputComponent->BindAxis("MoveUp_Gyro", this, &ACometPawn::MoveUpInput_Gyro);
	PlayerInputComponent->BindAxis("MoveRight_Gyro", this, &ACometPawn::MoveRightInput_Gyro);


	PlayerInputComponent->BindAction("Sync", IE_Pressed, this, &ACometPawn::SyncBeat);
	PlayerInputComponent->BindAction("Restart", IE_Pressed, this, &ACometPawn::ReloadLevel);

}


void ACometPawn::ThrustInput(float Val)
{
	float CurrentAcc = NaturalAcceleration;
	// Is there any input?
	bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);

	// Decelerate if thrust disabled 
	if (!bThrustEnabled)
	{
		CurrentAcc = -BrakeSphereDeceleration;
		if (!bHasInput)
		{
			SetThrustEnabled(true);
		}
	}
	else
	{
		if (bHasInput)
		{
			if (Val > 0)
			{
				CurrentAcc = Val * Acceleration;
			}
			else
			{
				CurrentAcc = Val * BrakeDeceleration;
			}
		}
	}

	// Calculate new speed
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
	// Clamp between MinSpeed and MaxSpeed
	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}

void ACometPawn::MoveUpInput(float Val)
{


	if (!bMoveUpEnabled)
	{
		Val = 0;
	}
	

	// Target pitch speed is based in input
	float TargetPitchSpeed = (Val * PitchSpeed * -1.f);
	
	// When steering, we decrease pitch slightly
	TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

	// Smoothly interpolate to target pitch speed
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void ACometPawn::MoveRightInput(float Val)
{
	if (!bMoveRightEnabled)
	{
		Val = 0;
	}

	if (FMath::Abs(GetActorRotation().Pitch) > MaxTurnPitchAngle)
	{
		Val = Val * .02f;
	}
	// Target yaw speed is based on input
	float TargetYawSpeed = (Val * YawSpeed);

	// Smoothly interpolate to target yaw speed
	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

	// Is there any left/right input?
	const bool bIsTurning = FMath::Abs(Val) > 0.2f;

	// If turning, yaw value is used to influence roll
	// If not turning, roll to reverse current roll value.
	float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (GetActorRotation().Roll * -2.f);

	// Smoothly interpolate roll speed
	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void ACometPawn::MoveUpInput_Gyro(float Val)
{
	if(bUseMotionControl)
	{
		MoveUpInput(Val);
	}
}

void ACometPawn::MoveRightInput_Gyro(float Val)
{
	if (bUseMotionControl) 
	{
		MoveRightInput(Val);
	}
}

void ACometPawn::MoveUpInput_Stick(float Val)
{
	if (bUseMotionControl) { return; }

	if (bInvertedAxis)
	{
		Val = -Val;
	}

	MoveUpInput(Val);
}

void ACometPawn::MoveRightInput_Stick(float Val)
{
	if (bUseMotionControl) { return; }
	MoveRightInput(Val);
}

void ACometPawn::DashInput()
{
	switch (MovementState)
	{
	// Is there any input?
	case EMovementEnum::ME_Normal:
		if (DashCharged > 0)
		{
			// Call event
			OnDash();
			MovementState = EMovementEnum::ME_Dashing;
		}
		break;
	case EMovementEnum::ME_Dashing:
	{

		if (DashCharged > 0)
		{
			float NewDashSpeed = CurrentDashSpeed + DashAcc * GetWorld()->GetDeltaSeconds();
			CurrentDashSpeed = FMath::Clamp(NewDashSpeed, MinSpeed, MaxDashSpeed);
			DashCharged -= GetWorld()->GetDeltaSeconds();
		}
		else
		{
			DashCharged = 0;
			MovementState = EMovementEnum::ME_DeDashing;
		}
		break;

	}

	case EMovementEnum::ME_DeDashing:
	{
		if (DashCharged > 0)
		{
			MovementState = EMovementEnum::ME_Dashing;
		}


		float NewDashSpeed = CurrentDashSpeed + DeDashingAcc * GetWorld()->GetDeltaSeconds();
		if (NewDashSpeed > 0)
		{
			CurrentDashSpeed = NewDashSpeed;
		}
		else
		{
			CurrentDashSpeed = 0;
			OnDeDash();
			MovementState = EMovementEnum::ME_Normal;
		}

		break;
	}
	default:
		break;
	}

}

void ACometPawn::SyncBeat()
{
	OnRequestSync.Broadcast();


	int32 BeatSwitchIndex = 0;
	FLinearColor ParticleColour = Colour;

	TArray<AActor*> OutOverlappingActors;
	GetOverlappingActors(OutOverlappingActors, AActor::StaticClass());
	if (OutOverlappingActors.Num() > 0)
	{
		for (auto* OutOverlappingActor : OutOverlappingActors)
		{
			if (OutOverlappingActor) 
			{
				auto* BeatComponent = OutOverlappingActor->FindComponentByClass<UBeatComponent>();
				if (BeatComponent)
				{
					if (BeatComponent->RequestMatchBeat(this))
					{
						BeatSwitchIndex = 1;
						ACometCompanion* Companion = Cast<ACometCompanion>(OutOverlappingActor);
						if (Companion != NULL)
						{
							ParticleColour = Companion->GetColour();
						}
					}
					else
					{
						BeatSwitchIndex = 2;
					}
				}

			}
		}
	}

	if (BeatAudio != NULL)
	{
		if (BeatAudio->Sound)
		{
			BeatAudio->SetIntParameter(FName("Switch"), BeatSwitchIndex);
			BeatAudio->Play();
		}
	}

	bool bShouldUseDirectionalBeatParticle = true;
	if (SunActor == NULL)
	{
		bShouldUseDirectionalBeatParticle = false;
	}
	else
	{
		ASunCompanion* SunCompanion = Cast<ASunCompanion>(SunActor);
		if (SunCompanion != NULL)
		{
			bShouldUseDirectionalBeatParticle = !SunCompanion->bIsFree;
		}
		else
		{
			bShouldUseDirectionalBeatParticle = false;
		}
	}

	if (bShouldUseDirectionalBeatParticle)
	{
		for (auto& Elem: MoodComp->MoodMap)
		{
			if (!Elem.Value)
			{
				bShouldUseDirectionalBeatParticle = false;
				break;
			}
		}
	}

	if (!bShouldUseDirectionalBeatParticle && BeatParticleTemplate != NULL)
	{
		UNiagaraComponent* BeatParticleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(BeatParticleTemplate, CometMesh, TEXT("None"), FVector::ZeroVector, CometMesh->GetComponentRotation(), EAttachLocation::KeepRelativeOffset, false);
		BeatParticleComp->SetAbsolute(false, true, false);
		BeatParticleComp->SetWorldRotation(CometMesh->GetComponentRotation());
		BeatParticleComp->SetNiagaraVariableLinearColor(TEXT("User.StartColour"), ParticleColour);
		BeatParticleComp->SetNiagaraVariableLinearColor(TEXT("User.EndColour"), FLinearColor(ParticleColour.R, ParticleColour.G, ParticleColour.B, 0));
		BeatParticleComp->OnSystemFinished.AddUniqueDynamic(this, &ACometPawn::DestoryNS);
	}
	else if(BeatParticleToSunTemplate != NULL)
	{
		UNiagaraComponent* BeatParticleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(BeatParticleToSunTemplate, CometMesh, TEXT("None"), FVector::ZeroVector, CometMesh->GetComponentRotation(), EAttachLocation::KeepRelativeOffset, false);
		BeatParticleComp->SetAbsolute(false, true, false);
		BeatParticleComp->SetWorldRotation(CometMesh->GetComponentRotation());
		BeatParticleComp->SetNiagaraVariableLinearColor(TEXT("User.StartColour"), ParticleColour);
		BeatParticleComp->SetNiagaraVariableLinearColor(TEXT("User.EndColour"), FLinearColor(ParticleColour.R, ParticleColour.G, ParticleColour.B, 0));
		if (SunActor != NULL)
		{
			FVector SunLocalLoc = CometMesh->GetComponentTransform().InverseTransformPosition(SunActor->GetActorLocation());
			BeatParticleComp->SetNiagaraVariableVec3(TEXT("User.ForceEnd"), SunLocalLoc);
		}
		BeatParticleComp->OnSystemFinished.AddUniqueDynamic(this, &ACometPawn::DestoryNS);
	}
	
}

void ACometPawn::ReloadLevel()
{
	UGameplayStatics::OpenLevel(this, "MainMenu", false);
}

void ACometPawn::RollForward(USceneComponent* Comp, float RollAmount)
{
	if (!Comp) { return; }
	FRotator DeltaRotation = FRotator(-1, 0, 0) * RollAmount;
	Comp->AddLocalRotation(DeltaRotation);
}

void ACometPawn::CheckChangeMesh(float Mass)
{
	int32 NextIndex = CurrentMeshIndex + 1;
	if (MassMeshArr.IsValidIndex(NextIndex) && Mass > MassMeshArr[NextIndex].ThresholdMass)
	{
		ChangeMeshPlease(NextIndex);
		CurrentMeshIndex = NextIndex;
	}

}

void ACometPawn::ChangeMesh(int32 MeshIndex)
{
	if (MassMeshArr.IsValidIndex(MeshIndex) && MassMeshArr[MeshIndex].StaticMesh != NULL)
	{
		CometMesh->SetStaticMesh(MassMeshArr[MeshIndex].StaticMesh);
	}
}

void ACometPawn::LockCameraOnActor(AActor* ActorToLock)
{
	if (ActorToLock == NULL)
	{
		return;
	}

	// FVector RelativeLocation = GetActorTransform().InverseTransformPosition(ActorToLock->GetActorLocation());
	FRotator WorldRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ActorToLock->GetActorLocation());
	SpringArm->SetWorldRotation(WorldRotation);


}

void ACometPawn::DestoryNS(UNiagaraComponent* NSToDestroy)
{
	if (!NSToDestroy) return;
	if (!NSToDestroy->IsValidLowLevel()) return;
	NSToDestroy->ConditionalBeginDestroy(); //instantly clears UObject out of memory
}


void ACometPawn::SetUseMotionControl(bool bInUse)
{
	bUseMotionControl = bInUse;
}

void ACometPawn::SetThrustEnabled(bool bInEnabled)
{
	if (bInEnabled != bThrustEnabled)
	{
		bThrustEnabled = bInEnabled;
	}
}
