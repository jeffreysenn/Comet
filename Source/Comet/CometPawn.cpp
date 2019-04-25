// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "CometPawn.h"

#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "CometPlayerController.h"


ACometPawn::ACometPawn()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Flying/Meshes/UFO.UFO"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create static mesh component
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());	// Set static mesh
	RootComponent = PlaneMesh;

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);	// Attach SpringArm to RootComponent
	SpringArm->TargetArmLength = 160.0f; // The camera follows at this distance behind the character	
	SpringArm->SocketOffset = FVector(0.f,0.f,60.f);
	SpringArm->bEnableCameraLag = false;	// Do not allow camera to lag
	SpringArm->CameraLagSpeed = 15.f;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);	// Attach the camera
	Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller

	// Set handling parameters
	Acceleration = 500.f;
	MaxSpeed = 4000.f;
	MinSpeed = 500.f;
	CurrentForwardSpeed = 500.f;
	DodgeSpeed = 500.f;
	OriginalPitch = PitchSpeed;
	OriginalYaw = YawSpeed;
}

void ACometPawn::Tick(float DeltaSeconds)
{
	// Call any parent class Tick implementation
	Super::Tick(DeltaSeconds);

	const FVector DashMove = FVector(CurrentDashSpeed * DeltaSeconds, 0.f, 0.f);
	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaSeconds, 0.f, 0.f) + DashMove;

	// Move plan forwards (with sweep so we stop when we collide with things)
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
	// Rotate plane
	AddActorLocalRotation(DeltaRotation);

}

void ACometPawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	if (!(GetIsDash() || OtherComp->GetCollisionObjectType() == ECC_Destructible))
	{
		// Deflect along the surface when we collide.
		FRotator CurrentRotation = GetActorRotation();
		SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
	}
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
	PlayerInputComponent->BindAxis("Dash", this, &ACometPawn::DashInput);

}

void ACometPawn::ThrustInput(float Val)
{
	// Is there any input?
	bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
	// If input is not held down, reduce speed
	float CurrentAcc = bHasInput ? (Val * Acceleration) : (-0.5f * Acceleration);
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

void ACometPawn::DashInput(float Val)
{
	bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
	switch (MovementState)
	{
	// Is there any input?
	case EMovementEnum::ME_Normal:
		if (bHasInput)
		{
			MovementState = EMovementEnum::ME_Charging;
		}
		break;
	case EMovementEnum::ME_Charging:
		// If is charging
		if (bHasInput)
		{
			// Call charge event
			OnCharge();

			// Increase charge
			float CurrentCharge = Val * DashChargeSpeed * GetWorld()->GetDeltaSeconds();
			DashCharged = FMath::Min(MaxDashCharge, DashCharged + CurrentCharge);

			// Slow down the movement
			float NewForwardSpeed = CurrentForwardSpeed + DashChargingAcc * GetWorld()->GetDeltaSeconds();
			CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
		}
		else
		{
			MovementState = EMovementEnum::ME_Dashing;
		}
		break;
	case EMovementEnum::ME_Dashing:
		// Call event
		OnDash();
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
	case EMovementEnum::ME_DeDashing:
		OnDeDash();

		float NewDashSpeed = CurrentDashSpeed + DeDashingAcc * GetWorld()->GetDeltaSeconds();
		if (NewDashSpeed > 0)
		{
			CurrentDashSpeed = NewDashSpeed;
		}
		else
		{
			CurrentDashSpeed = 0;
			MovementState = EMovementEnum::ME_Normal;
		}

		//if (CurrentDeDashingTime < DeDashingAcc)
		//{
		//	CurrentDashSpeed = FMath::Lerp(CurrentDashSpeed, 0.f, CurrentDeDashingTime / DeDashingAcc);
		//	CurrentDeDashingTime += GetWorld()->GetDeltaSeconds();
		//}
		//else
		//{
		//	CurrentDeDashingTime = 0;
		//	MovementState = EMovementEnum::ME_Normal;
		//}

		break;
	}
	


	////if input was pressed
	//if (bHasInput) 
	//{
	//	bIsDashing = true;
	//	CurrentDash = Val * DashSpeed;
	//	YawSpeed = YawDashSpeed;
	//	PitchSpeed = PitchDashSpeed;
	//}
	//else
	//{
	//	CurrentDash = DashSpeed * 0.f;
	//	YawSpeed = OriginalYaw;
	//	PitchSpeed = OriginalPitch;
	//	CurrentDashSpeed = FMath::Clamp(MinSpeed, MinSpeed, MaxSpeed);
	//}

	//// Calculate new speed
	//float NewForwardSpeed = CurrentDashSpeed + (GetWorld()->GetDeltaSeconds() * CurrentDash);
	//// Clamp between MinSpeed and MaxSpeed
	//CurrentDashSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
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
