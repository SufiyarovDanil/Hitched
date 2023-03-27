/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "VincentLeaningComponent.generated.h"


class AVincentBloodberry;

/* Movement state enumerator contains all possible states */
UENUM(BlueprintType)
enum class ELeanState : uint8
{
	Left		UMETA(DisplayName = "Left Side"),
	Right		UMETA(DisplayName = "Right Side"),
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HITCHED_API UVincentLeaningComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVincentLeaningComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetLeanSide(ELeanState LeanState, bool NewSide);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	
#pragma region FUNCTIONS

	void TickLeaning(const float DeltaTime);

#pragma endregion

#pragma region VARIABLES

	UPROPERTY(VisibleAnywhere, Category = "Owner")
	AVincentBloodberry* OwningCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Lean | Side")
	bool bIsLeaningLeft;

	UPROPERTY(VisibleAnywhere, Category = "Lean | Side")
	bool bIsLeaningRight;

	UPROPERTY(VisibleAnywhere, Category = "Max Lean Distance")
	float MaxLeanDistance;

	UPROPERTY(VisibleAnywhere, Category = "Max Lean Angle")
	float MaxLeanAngle;

	UPROPERTY(VisibleAnywhere, Category = "Lean | offset of Y axis")
	float LeanOffsetY;

	UPROPERTY(VisibleAnywhere, Category = "Lean | offset roll")
	float LeanOffsetRoll;

	UPROPERTY(VisibleAnywhere, Category = "Lean | speed")
	float LeanSpeed;

#pragma endregion

};
