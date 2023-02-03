// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "VincentLeaningComponent.generated.h"


class AVincentBloodberry;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HITCHED_API UVincentLeaningComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVincentLeaningComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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

	UPROPERTY(VisibleAnywhere, Category = "Max Lean Distance")
	float MaxLeanDistance;

	UPROPERTY(VisibleAnywhere, Category = "Max Lean Angle")
	float MaxLeanAngle;

	UPROPERTY(VisibleAnywhere, Category = " Lean offset of Y axis")
	float LeanOffsetY;

	UPROPERTY(VisibleAnywhere, Category = "Lean offset roll")
	float LeanOffsetRoll;

	UPROPERTY(VisibleAnywhere, Category = "Lean speed")
	float LeanSpeed;

#pragma endregion

};
