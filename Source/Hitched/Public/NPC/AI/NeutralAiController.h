/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NeutralAiController.generated.h"


/**
 * 
 */
UCLASS()
class HITCHED_API ANeutralAiController : public AAIController
{
	GENERATED_BODY()

private:

	/*  */
	UPROPERTY(VisibleAnywhere, Category = "State")
	bool bIsWalking;

	/*  */
	UPROPERTY(VisibleAnywhere, Category = "Timer")
	float MinWaitTime;

	/*  */
	UPROPERTY(VisibleAnywhere, Category = "Timer")
	float MaxWaitTime;

	/*  */
	UPROPERTY()
	FVector DestinationLocation;

public:

	ANeutralAiController();

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

private:

	/*  */
	void TickWalking();

	/*  */
	void StartWalking();

	/*  */
	void StopWalking();

	/*  */
	void PrepareForNextWalking();

};
