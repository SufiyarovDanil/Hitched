/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#pragma once

#include "CoreMinimal.h"
#include "ActionableActorBase.h"
#include "GrabbableActor.generated.h"

/**
 * 
 */
UCLASS()
class HITCHED_API AGrabbableActor : public AActionableActorBase
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	AGrabbableActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region INTERFACE_OVERRIDE

	virtual void DoAction_Implementation(AActor* Caller) override;

	virtual void StartFocus_Implementation() override;

	virtual void EndFocus_Implementation() override;

#pragma endregion

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
