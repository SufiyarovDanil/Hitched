/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "Vincent/VincentMovementComponent.h"
#include "Vincent/VincentBloodberry.h"


UVincentMovementComponent::UVincentMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Movement settings
	RotationRate = FRotator(0.f, 540.f, 0.f);
	AirControl = 0.f;
	CrouchedHalfHeight = 50.f;
	MaxWalkSpeedCrouched = 80.f;
	SetWalkableFloorAngle(60);

	// Movement Nav properties
	NavAgentProps.bCanCrouch = true;
	NavAgentProps.bCanFly = false;
	NavAgentProps.bCanJump = true;
	NavAgentProps.bCanWalk = true;
	NavAgentProps.bCanSwim = false;

	MovementState.bCanCrouch = true;
	MovementState.bCanFly = false;
	MovementState.bCanJump = true;
	MovementState.bCanWalk = true;
	MovementState.bCanSwim = false;

	bCanWalkOffLedges = true;
	bCanWalkOffLedgesWhenCrouching = true;

	// Inertia params
	MaxAcceleration = 1024.f;
	BrakingFrictionFactor = 0.f;
	GroundFriction = 4.f;
	BrakingDecelerationWalking = 512.f;

	PerchRadiusThreshold = 20.f;
}


void UVincentMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TickTilt(DeltaTime);
}


void UVincentMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<AVincentBloodberry>(GetOwner());
}


/* This function overrided for jumping in crouch state */
bool UVincentMovementComponent::CanAttemptJump() const
{
	// Falling included for double-jump and non-zero jump hold time, but validated by character.
	return IsJumpAllowed() && (IsMovingOnGround() || IsFalling());
}


void UVincentMovementComponent::TickTilt(const float DeltaTime)
{
	if (!OwningCharacter)
	{
		return;
	}

	const FRotator CurrentControlRotation = OwningCharacter->GetController()->GetControlRotation();

	const float TiltAxis = OwningCharacter->GetInputAxisValue(TEXT("Move Right")); // Hardcoded shit
	const float NextInterpTiltOffset = FMath::FInterpTo(TiltOffset, MaxTiltAngle * FMath::Sign(TiltAxis), DeltaTime, TiltSpeed);
	const float Adjust = NextInterpTiltOffset - TiltOffset;

	TiltOffset = NextInterpTiltOffset;

	const FRotator NewControlRotation
	(
		CurrentControlRotation.Pitch,
		CurrentControlRotation.Yaw,
		CurrentControlRotation.Roll + Adjust
	);

	OwningCharacter->GetController()->SetControlRotation(NewControlRotation);
}
