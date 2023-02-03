/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VincentMovementComponent.generated.h"

class AVincentBloodberry;

/**
 * 
 */
UCLASS()
class HITCHED_API UVincentMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	UVincentMovementComponent();

	bool CanAttemptJump() const override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	virtual void BeginPlay() override;

private:

#pragma region FUNCTIONS

	void TickTilt(const float DeltaTime);

#pragma endregion

#pragma region VARIABLES

	AVincentBloodberry* OwningCharacter = nullptr;

#pragma endregion

#pragma region TILT

	float MaxTiltAngle = 0.5f;

	float TiltSpeed = 5.f;

	float TiltOffset = 0.f;

#pragma endregion

};
