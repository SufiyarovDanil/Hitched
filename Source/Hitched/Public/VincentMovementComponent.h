/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VincentMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class HITCHED_API UVincentMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	UVincentMovementComponent();

	bool CanAttemptJump() const override;
};
