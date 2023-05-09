/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "NPC/AI/NeutralAiController.h"
#include "NavigationSystem.h"


ANeutralAiController::ANeutralAiController()
{
	bIsWalking = false;
	MinWaitTime = 6.f;
	MaxWaitTime = 15.f;
}


void ANeutralAiController::BeginPlay()
{
	Super::BeginPlay();

	PrepareForNextWalking();
}


void ANeutralAiController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsWalking)
	{
		TickWalking();
	}
}


void ANeutralAiController::TickWalking()
{
	EPathFollowingRequestResult::Type RequestResult = MoveToLocation
	(
		DestinationLocation
	);

	if (RequestResult == EPathFollowingRequestResult::Type::AlreadyAtGoal)
	{
		StopWalking();
	}
}


void ANeutralAiController::StartWalking()
{
	APawn* ControlledPawn = GetPawn();

	if (!ControlledPawn)
	{
		return;
	}

	UNavigationSystemV1* NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	FVector PawnLocation = ControlledPawn->GetActorLocation();
	FNavLocation Destination;

	const bool bFoundLocation = NavigationArea->GetRandomReachablePointInRadius
	(
		PawnLocation,
		1000.f,
		Destination
	);

	if (!bFoundLocation)
	{
		return;
	}

	DestinationLocation = Destination.Location;

	bIsWalking = true;
}


void ANeutralAiController::StopWalking()
{
	bIsWalking = false;

	PrepareForNextWalking();
}


void ANeutralAiController::PrepareForNextWalking()
{
	FTimerHandle StandingTimerHandle;
	const float StandingTime = FMath::RandRange(MinWaitTime, MaxWaitTime);

	GetWorldTimerManager()
		.SetTimer(StandingTimerHandle, this, &ANeutralAiController::StartWalking, StandingTime, false);
}
