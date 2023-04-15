/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#pragma once

#include "CoreMinimal.h"
#include "ActionableActorBase.h"
#include "PickableActor.generated.h"


class AVincentBloodberry;

/**
 * The actor type you can add to inventory
 */
UCLASS()
class HITCHED_API APickableActor : public AActionableActorBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Picking Character")
	AVincentBloodberry* PickingCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* PickableMesh = nullptr;

private:

	UPROPERTY(VisibleAnywhere, Category = "Parameters")
	bool bIsPicked;

	UPROPERTY(VisibleAnywhere, Category = "Parameters")
	bool bIsPickedUpOnce;

	/* the time of magnetise to character (this actor will be added to inventory in 'PickTime' seconds) */
	UPROPERTY(VisibleAnywhere, Category = "Parameters")
	float PickTime;

	UPROPERTY(VisibleAnywhere, Category = "Parameters")
	float PickTimeProgress;

	UPROPERTY()
	FVector PickStartLocation;

public:

	APickableActor();

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

	void TickMagnetisingToCharacter(const float DeltaTime);

	void AddToInventory();

	virtual void OnAddingToInventory();

};
