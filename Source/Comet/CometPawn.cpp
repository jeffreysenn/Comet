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
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "NiagaraComponent.h"
#include "CometCompanion.h"
#include "SunCompanion.h"
#include "MassComponent.h"


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


	BeatNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BeatNiagara0"));
	BeatNiagara->SetupAttachment(RootComponent);

	BeatAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("BeatAudio0"));
	BeatAudio->SetupAttachment(RootComponent);

	MassComp = CreateDefaultSubobject<UMassComponent>(TEXT("MassComp0"));
	MassComp->OnMassChanged.AddDynamic(this, &ACometPawn::CheckChangeMesh);

	// Set handling parameters
	OriginalPitch = PitchSpeed;
	OriginalYaw = YawSpeed;
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

	// Update particle system user variables
	if (BeatNiagara != NULL)
	{
		// Set Niagara spawning rotation quat
		FQuat MeshRoationQuat = CometMesh->GetComponentRotation().Quaternion();
		FQuat LocalMeshRotationQuat = BeatNiagara->GetComponentTransform().InverseTransformRotation(MeshRoationQuat);
		BeatNiagara->SetNiagaraVariableQuat("User.SpawningQuat", LocalMeshRotationQuat);

		//Set Niagara sprite size
		FVector2D FinalSpriteSize = ParticleSpriteSize * GetActorScale().X;
		BeatNiagara->SetNiagaraVariableVec2("User.SpriteSize", FinalSpriteSize);

		if (ClosestCompanion != NULL)
		{
			BeatNiagara->SetNiagaraVariableBool("User.bUseLineAttractionForce", true);
			FVector ClosestCompanionRelativeLocation = BeatNiagara->GetComponentTransform().InverseTransformPosition(ClosestCompanion->GetActorLocation());
			BeatNiagara->SetNiagaraVariableVec3("User.ClosestCompanionLocation", ClosestCompanionRelativeLocation);
		}
		// If ClosestCompanion == NULL, do not apply line force
		else
		{
			BeatNiagara->SetNiagaraVariableBool("User.bUseLineAttractionForce", false);
		}

		BeatNiagara->SetNiagaraVariableBool("User.bShouldSpawn", bShouldSpawnBeatParticle);
		if (bShouldSpawnBeatParticle)
		{
			bShouldSpawnBeatParticle = false;
		}
	}

	
}

void ACometPawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

		// Deflect along the surface when we collide.
		FRotator CurrentRotation = GetActorRotation();
		SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.5f));
}

void ACometPawn::RequestDash(float DeltaCharge)
{
	DashCharged = FMath::Min(MaxDashCharge, DashCharged + DeltaCharge);
}


void ACometPawn::BeginPlay()
{
	Super::BeginPlay();

	CurrentMeshIndex = 0;
	ChangeMesh(CurrentMeshIndex);
}

void ACometPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // Check if PlayerInputComponent is valid (not NULL)
	check(PlayerInputComponent);

	// Bind our control axis' to callback functions
	PlayerInputComponent->BindAxis("Thrust", this, &ACometPawn::ThrustInput);
	PlayerInputComponent->BindAxis("MoveUp", this, &ACometPawn::MoveUpInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACometPawn::MoveRightInput);
	PlayerInputComponent->BindAxis("ThrustX", this, &ACometPawn::DodgeInput);
	//PlayerInputComponent->BindAxis("Dash", this, &ACometPawn::DashInput);


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
	// Target pitch speed is based in input
	float TargetPitchSpeed = (Val * PitchSpeed * -1.f);
	
	// When steering, we decrease pitch slightly
	TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

	// Smoothly interpolate to target pitch speed
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void ACometPawn::MoveRightInput(float Val)
{
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

void ACometPawn::DodgeInput(float Val)
{
	// Target yaw speed is based on input
	float TargetYawSpeed = (Val * DodgeSpeed);

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

void ACometPawn::SyncBeat()
{
	if (BeatNiagara != NULL)
	{
		ClosestCompanion = FindClosestCompanion();

		// UE_LOG(LogTemp, Warning, TEXT("ClosestCompanion: %s"), ClosestCompanion != NULL ? *ClosestCompanion->GetName() : TEXT("Bullshit"));

		bShouldSpawnBeatParticle = true;

	}

	if (BeatAudio != NULL)
	{
		if (BeatAudio->Sound)
		{
			BeatAudio->Play();
		}
	}

	TArray<AActor*> OutOverlappingActors;
	GetOverlappingActors(OutOverlappingActors, AActor::StaticClass());
	if (OutOverlappingActors.Num() == 0) { return; }
	else
	{
		for (auto* OutOverlappingActor : OutOverlappingActors)
		{
			//auto* Companion = Cast<ACometCompanion>(OutOverlappingActor);
			//if (Companion)
			//{
				//if (!Companion->bIsFree)
				//{
			if (OutOverlappingActor) 
			{
					auto* BeatComponent = OutOverlappingActor->FindComponentByClass<UBeatComponent>();
					if (BeatComponent)
					{
						BeatComponent->RequestMatchBeat(this);
					}

			}
				//}
			//}
		}
	}
}

void ACometPawn::ReloadLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void ACometPawn::RollForward(USceneComponent* Comp, float RollAmount)
{
	if (!Comp) { return; }
	FRotator DeltaRotation = FRotator(-1, 0, 0) * RollAmount;
	Comp->AddLocalRotation(DeltaRotation);
}

ACometCompanion* ACometPawn::FindClosestCompanion()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACometCompanion::StaticClass(), OutActors);
	float MinDist = TNumericLimits<float>::Max();
	ACometCompanion* MyClosestCompanion = nullptr;
	for (AActor* Actor : OutActors)
	{
		ACometCompanion* Companion = Cast<ACometCompanion>(Actor);
		if (Companion)
		{
			// Ignore the already freed companions
			if (Companion->bIsFree)
			{
				continue;
			}
			// If the MyClosestCompanion is not set, set it to the sun
			else if (MyClosestCompanion == nullptr && Companion->IsA(ASunCompanion::StaticClass()))
			{
				MyClosestCompanion = Companion;
				continue;
			}

			else
			{
				float Dist = FVector::Dist(GetActorLocation(), Companion->GetActorLocation());
				if (Dist < MinDist)
				{
					MinDist = Dist;
					MyClosestCompanion = Companion;
				}
			}
		}

	}

	// TODO: properly activate the sun beat
	// if only the sun is left, activate the sun beat
	if (MyClosestCompanion != NULL && MyClosestCompanion->IsA(ASunCompanion::StaticClass()))
	{
		ASunCompanion* SunCompanion = Cast<ASunCompanion>(MyClosestCompanion);
		SunCompanion->SetBeatAndParticleActive(true);
	}

	return MyClosestCompanion;
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
