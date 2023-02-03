// Fill out your copyright notice in the Description page of Project Settings.


#include "Vincent/VincentLeaningComponent.h"
#include "Vincent/VincentBloodberry.h"


// Sets default values for this component's properties
UVincentLeaningComponent::UVincentLeaningComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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

	const float LeanAxis = FMath::Sign(OwningCharacter->GetInputAxisValue(TEXT("Lean")));

	// Calculating relative location
	const FVector ComponentLocation = GetRelativeLocation();

	LeanOffsetY = FMath::FInterpTo(LeanOffsetY, MaxLeanDistance * LeanAxis, DeltaTime, LeanSpeed);

	const FVector NewComponentLocation(ComponentLocation.X, LeanOffsetY, ComponentLocation.Z);

	SetRelativeLocation(NewComponentLocation);

	// Calculating Character control rotation
	const FRotator CurrentControlRotation = OwningCharacter->GetController()->GetControlRotation();

	const float NextInterpLeanOffsetRoll = FMath::FInterpTo(LeanOffsetRoll, MaxLeanAngle * LeanAxis, DeltaTime, LeanSpeed);
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
