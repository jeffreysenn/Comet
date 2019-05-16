// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CometPawn.generated.h"

UENUM(BlueprintType)
enum class EMovementEnum : uint8
{
	ME_Normal,
	ME_Charging,
	ME_Dashing,
	ME_DeDashing
};


UCLASS(Config=Game)
class COMET_API ACometPawn : public APawn
{
	GENERATED_BODY()


public:
	ACometPawn();


public:
	/** Scene component, root */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* RootCollider;

	/** StaticMesh component that will be the visuals for our flying pawn */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CometMesh;

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(Category = Particle, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent* BeatNiagara;

	UPROPERTY(Category = Audio, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* BeatAudio;


protected:

	/** How quickly forward speed changes */
	UPROPERTY(Category = Movement, EditAnywhere)
	float Acceleration = 800;

	UPROPERTY(Category = Movement, EditAnywhere)
	float NaturalAcceleration = -1600;

	UPROPERTY(Category = Movement, EditAnywhere)
	float BrakeDeceleration = 3000;

	UPROPERTY(Category = Movement, EditAnywhere)
	float BrakeSphereDeceleration = 10000;

	/** How quickly pawn can steer */
	UPROPERTY(Category = Movement, EditAnywhere)
	float YawSpeed = 90;

	UPROPERTY(Category = Movement, EditAnywhere)
	float PitchSpeed = 90;

	UPROPERTY(Category = Movement, EditAnywhere)
	float MaxPitchAngle = 85;

	/** Max forward speed */
	UPROPERTY(Category = Pitch, EditAnywhere)
	float MaxSpeed = 5000;

	/** Min forward speed */
	UPROPERTY(Category = Yaw, EditAnywhere)
	float MinSpeed = 50;

	/** How quickly forward dash speed is */
	UPROPERTY(Category = Dash, EditAnywhere)
	float MaxDashSpeed = 20000;

	UPROPERTY(Category = Dash, EditAnywhere)
	float DashAcc = 8000;

	UPROPERTY(Category = Dash, EditAnywhere)
	float DashChargeSpeed = .5f;

	UPROPERTY(Category = Dash, EditAnywhere)
	float MaxDashCharge = 3;

	UPROPERTY(Category = Dash, EditAnywhere)
	float DashChargingAcc = -5000;

	UPROPERTY(Category = Dash, EditAnywhere)
	float DeDashingAcc = -3000;

	/*How sharp the dodge speed is*/
	UPROPERTY(Category = Movement, EditAnywhere)
	float DodgeSpeed = 500;

	/** How sensitive the comet moves sideways */
	UPROPERTY(Category = Movement, EditAnywhere)
	float PitchDashSpeed = 110;

	UPROPERTY(Category = Movement, EditAnywhere)
	float YawDashSpeed = 110;

	UPROPERTY(Category = CometMesh, EditAnywhere)
	float RollMod = 0.0002f;

	UPROPERTY(Category = Input, EditAnywhere)
		float MaxDustCharge = 0;

	UPROPERTY(Category = Input, EditAnywhere, BlueprintReadWrite)
	bool bUseMotionControl = true;

	UPROPERTY(Category = Particle, EditAnywhere)
	FVector2D ParticleSpriteSize = FVector2D(6.f, 8.f);

private:
	/** Current forward speed */
	float CurrentForwardSpeed;

	/** Current yaw speed */
	float CurrentYawSpeed;

	/** Current pitch speed */
	float CurrentPitchSpeed;

	/** Current roll speed */
	float CurrentRollSpeed;

	/*Current Dash Speed*/
	float CurrentDashSpeed;

	/*Current Dodge Speed*/
	float CurrentDodgeSpeed;

	/*Saving original yaw and pitch speeds for the dash input*/
	float OriginalPitch;
	float OriginalYaw;

	/*just to test some stuff*/
	bool bIsDashing;

	/*activated dashing on collecting*/
	bool bHasActivatedDash = false;

	/*Current dust charge*/
	float DustCharge = 0;

	/** Dash Charged Amount */
	UPROPERTY(VisibleAnywhere)
	float DashCharged = 0;

	/** Movement State */
	UPROPERTY(VisibleAnywhere)
	EMovementEnum MovementState = EMovementEnum::ME_Normal;

	float CurrentDeDashingTime = 0;

	bool bShouldSpawnBeatParticle = false;

	UPROPERTY()
	class ACometCompanion* ClosestCompanion = nullptr;

	bool bThrustEnabled = true;

public:
	// Begin AActor overrides
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	// End AActor overrides

	/** Returns PlaneMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetPlaneMesh() const { return CometMesh; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }

	/* setting the dash time and editing in blueprint*/
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsDash() const { return bIsDashing; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE EMovementEnum GetMovementState() const { return MovementState; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetUseMotionControll() const {return bUseMotionControl;}

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetDustCharge() const { return DustCharge; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetHasActivatedDash() const { return bHasActivatedDash; }

	UFUNCTION(BlueprintCallable)
	void SetUseMotionControl(bool bInUse);

	UFUNCTION(BlueprintCallable)
	void SetHasActivatedDash(bool bHasUsed);

	UFUNCTION(BlueprintCallable)
	void SetDustCharge(float Val);

	UFUNCTION(BlueprintCallable)
	void SetThrustEnabled(bool bInEnabled);

protected:

	// Begin APawn overrides
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override; // Allows binding actions/axes to functions
	// End APawn overrides

	/** Bound to the thrust axis */
	void ThrustInput(float Val);
	
	/** Bound to the vertical axis */
	void MoveUpInput(float Val);

	/** Bound to the horizontal axis */
	void MoveRightInput(float Val);


	void MoveUpInput_Gyro(float Val);

	void MoveRightInput_Gyro(float Val);

	void MoveUpInput_Stick(float Val);

	void MoveRightInput_Stick(float Val);


	/** Bound to the Dust collection */
	void DashInput();

	/*Bound to the Dodge axis*/
	void DodgeInput(float Val);

	/** Bound to the sync action */
	void SyncBeat();

	/** Bound to restart action */
	void ReloadLevel();

	void RollForward(USceneComponent* Comp, float RollAmount);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDash();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeDash();

	UFUNCTION(BlueprintImplementableEvent)
	void OnCharge();

	class ACometCompanion* FindClosestCompanion();

};


