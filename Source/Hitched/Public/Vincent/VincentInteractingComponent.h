/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VincentInteractingComponent.generated.h"


class AVincentBloodberry;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HITCHED_API UVincentInteractingComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	UVincentInteractingComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TraceForward();

	void InteractButtonPressed();

protected:

	// Called when the game starts
	virtual void BeginPlay() override;
	
private:

	AVincentBloodberry* OwningCharacter = nullptr;

#pragma region VARIABLES

	AActor* FocusedInteractableActor = nullptr;

	float TraceDistance;

#pragma endregion

#pragma region FUNCTIONS

	void TryToEndFocus();

	void TryToStartFocus(AActor* NewActor);

#pragma endregion

};
