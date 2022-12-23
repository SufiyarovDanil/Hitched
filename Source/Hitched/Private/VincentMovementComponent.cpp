/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "VincentMovementComponent.h"


UVincentMovementComponent::UVincentMovementComponent()
{
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


bool UVincentMovementComponent::CanAttemptJump() const
{
	// Falling included for double-jump and non-zero jump hold time, but validated by character.
	return IsJumpAllowed() && (IsMovingOnGround() || IsFalling());
}
