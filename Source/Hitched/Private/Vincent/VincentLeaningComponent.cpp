/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "Vincent/VincentLeaningComponent.h"
#include "Vincent/VincentBloodberry.h"


// Sets default values for this component's properties
UVincentLeaningComponent::UVincentLeaningComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bIsLeaningLeft = false;
	bIsLeaningRight = false;
	MaxLeanDistance = 50.f;
	MaxLeanAngle = 5.f;
	LeanOffsetY = 0.f;
	LeanOffsetRoll = 0.f;
	LeanSpeed = 5.f;
}


// Called when the game starts
void UVincentLeaningComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<AVincentBloodberry>(GetOwner());	
}


// Called every frame
void UVincentLeaningComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TickLeaning(DeltaTime);
}


void UVincentLeaningComponent::TickLeaning(const float DeltaTime)
{
	if (!OwningCharacter)
	{
		return;
	}

	//const float LeanAxis = FMath::Sign(OwningCharacter->GetInputAxisValue(TEXT("Lean")));

	// Calculating relative location
	const FVector ComponentLocation = GetRelativeLocation();

	float LeanSide = 0.f;

	if (bIsLeaningLeft)
	{
		LeanSide -= 1.f;
	}

	if (bIsLeaningRight)
	{
		LeanSide += 1.f;
	}

	LeanOffsetY = FMath::FInterpTo(LeanOffsetY, MaxLeanDistance * LeanSide, DeltaTime, LeanSpeed);

	const FVector NewComponentLocation(ComponentLocation.X, LeanOffsetY, ComponentLocation.Z);

	SetRelativeLocation(NewComponentLocation);

	// Calculating Character control rotation
	const FRotator CurrentControlRotation = OwningCharacter->GetController()->GetControlRotation();

	const float NextInterpLeanOffsetRoll = FMath::FInterpTo(LeanOffsetRoll, MaxLeanAngle * LeanSide, DeltaTime, LeanSpeed);
	const float Adjust = NextInterpLeanOffsetRoll - LeanOffsetRoll;

	LeanOffsetRoll = NextInterpLeanOffsetRoll;

	const FRotator NewControlRotation
	(
		CurrentControlRotation.Pitch,
		CurrentControlRotation.Yaw,
		CurrentControlRotation.Roll + Adjust
	);

	OwningCharacter->GetController()->SetControlRotation(NewControlRotation);
}


void UVincentLeaningComponent::SetLeanSide(ELeanState LeanState, bool NewSide)
{
	switch (LeanState)
	{
	case ELeanState::Left:
		bIsLeaningLeft = NewSide;
		break;
	case ELeanState::Right:
		bIsLeaningRight = NewSide;
		break;
	default:
		break;
	}
}
