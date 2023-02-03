/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/

#pragma once

#include "CoreMinimal.h"
#include "MatineeCameraShake.h"
#include "LandingCameraShake.generated.h"

/**
 * Ñamera shake to give the effect of landing on the any floor
 */
UCLASS()
class HITCHED_API ULandingCameraShake : public UMatineeCameraShake
{
	GENERATED_BODY()

	ULandingCameraShake();

};
