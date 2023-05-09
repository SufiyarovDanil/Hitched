/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VincentActioningComponent.generated.h"


class AVincentBloodberry;
class AActionableActorBase;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HITCHED_API UVincentActioningComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	UVincentActioningComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TraceForward();

	void ActionButtonPressed();

protected:

	// Called when the game starts
	virtual void BeginPlay() override;
	
private:

	UPROPERTY(VisibleAnywhere, Category = "Component Owner")
	AVincentBloodberry* OwningCharacter = nullptr;

#pragma region VARIABLES

	UPROPERTY(VisibleAnywhere, Category = "Actionable Actor Target")
	AActor* FocusedActionableActor = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Interact Distance")
	float TraceDistance;

#pragma endregion

#pragma region FUNCTIONS

	void TryToEndFocus();

	void TryToStartFocus(AActor* NewActor);

#pragma endregion

	UPROPERTY(VisibleAnywhere, Category = "Grabbing")
	bool bIsGrabbing;

	UPROPERTY(VisibleAnywhere, Category = "Grabbing")
	float MaxDeflectionDistance;

	void Grab();

	void Drop();

	void TickGrabbableActor(const float DeltaTime);

};
