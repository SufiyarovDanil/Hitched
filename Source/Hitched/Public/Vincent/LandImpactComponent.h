/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LandImpactComponent.generated.h"


class UTimelineComponent;
class UCurveVector;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HITCHED_API ULandImpactComponent : public USceneComponent
{
	GENERATED_BODY()

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UTimelineComponent* LandImpactTL = nullptr;

private:

	UPROPERTY(VisibleAnywhere, Category = "Timeline")
	UCurveVector* LandCurve = nullptr;

public:

	// Sets default values for this component's properties
	ULandImpactComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartAnimation();

private:

	UFUNCTION()
	void LandTLProgress(const FVector Location);

};
