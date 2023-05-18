/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "NPC/AI/EnemyAIController.h"
#include "Vincent/VincentBloodberry.h"
#include "NPC/EnemyNpc.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "NavigationSystem.h"


AEnemyAIController::AEnemyAIController()
{
	AiPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
	AiSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));

	AiSight->SightRadius = 1000.f;
	AiSight->LoseSightRadius = AiSight->SightRadius + 500.f;
	AiSight->PeripheralVisionAngleDegrees = 60.f;
	AiSight->DetectionByAffiliation.bDetectEnemies = true;
	AiSight->DetectionByAffiliation.bDetectNeutrals = true;
	AiSight->DetectionByAffiliation.bDetectFriendlies = true;


	AiPerception->ConfigureSense(*AiSight);
	AiPerception->SetDominantSense(AiSight->GetSenseImplementation());

	CurrentState = ENpcState::Calm;
	SightLevel = 0.f;
	SightFillSpeed = 0.75f;
}


void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentState)
	{
	case ENpcState::Calm:
		TickCalmState(DeltaTime);
		break;
	case ENpcState::Caution:
		TickCautionState(DeltaTime);
		break;
	case ENpcState::Battle:
		TickBattleState(DeltaTime);
		break;
	default:
		break;
	}
}


void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<AEnemyNpc>(GetPawn());
}


void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!AiPerception)
	{
		return;
	}

	/*AiPerception
		->OnTargetPerceptionUpdated
		.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdatedDelegate);*/
}


void AEnemyAIController::TickCalmState(float DeltaTime)
{
	// if (GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("Calm")));
	// }

	if (IsPlayerVisible())
	{
		CurrentState = ENpcState::Caution;
	}
}


void AEnemyAIController::TickCautionState(float DeltaTime)
{
	// if (GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("Caution")));
	// }

	if (IsPlayerVisible())
	{
		SightLevel = SightLevel + (SightFillSpeed * DeltaTime);
	}
	else
	{
		SightLevel = SightLevel - (SightFillSpeed * DeltaTime);
	}

	if (SightLevel >= 1.f)
	{
		CurrentState = ENpcState::Battle;
	}

	if (SightLevel <= 0.f)
	{
		CurrentState = ENpcState::Calm;
	}
}


void AEnemyAIController::TickBattleState(float DeltaTime)
{
	// if (GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("Battle")));
	// }

	TArray<AActor*> OutActors;

	AiPerception->GetPerceivedActors(
		AiSight->GetSenseImplementation(),
		OutActors);

	for (AActor* PerceivedActor : OutActors)
	{
		AVincentBloodberry* Vince = Cast<AVincentBloodberry>(PerceivedActor);

		if (!Vince)
		{
			continue;
		}

		const FVector VinceLoc = Vince->GetActorLocation();

		EPathFollowingRequestResult::Type RequestResult = MoveToLocation(VinceLoc, 100.f);

		if (RequestResult == EPathFollowingRequestResult::Type::AlreadyAtGoal)
		{
			OwningCharacter->Attack();
		}

		return;
	}

	CurrentState = ENpcState::Caution;
}


void AEnemyAIController::OnPerceptionUpdatedDelegate(AActor* Actor, FAIStimulus Stimulus)
{
	AVincentBloodberry* Vincent = Cast<AVincentBloodberry>(Actor);

	if (!Vincent)
	{
		return;
	}

	const FVector MyLoc = GetOwner()->GetActorLocation();
	const FVector Loc = Vincent->GetActorLocation();
	const float DistanceToVince = FVector::Distance(MyLoc, Loc);
	const float VinceLightLevel = Vincent->GetLightLevel();

	const bool bCanSeeVince = FMath::Lerp(0.f, AiSight->SightRadius, VinceLightLevel) > DistanceToVince;

	if (bCanSeeVince)
	{
		CurrentState = ENpcState::Caution;
	}
}


bool AEnemyAIController::IsPlayerVisible()
{
	TArray<AActor*> OutActors;

	AiPerception->GetCurrentlyPerceivedActors(
		AiSight->GetSenseImplementation(),
		OutActors);

	if (OutActors.Num() == 0)
	{
		return false;
	}

	for (AActor* PerceivedActor : OutActors)
	{
		AVincentBloodberry* Vince = Cast<AVincentBloodberry>(PerceivedActor);

		if (!Vince)
		{
			continue;
		}

		return CheckVisiblity(Vince->GetActorLocation(), Vince->GetLightLevel());
	}

	return false;
}


bool AEnemyAIController::CheckVisiblity(FVector VinceLoc, const float LightLevel)
{
	const FVector MyLoc = GetPawn()->GetActorLocation();
	const float DistanceToVince = FVector::Distance(MyLoc, VinceLoc);
	const bool bCanSeeVince = FMath::Lerp(0.f, AiSight->SightRadius, LightLevel) > DistanceToVince;

	return bCanSeeVince;
}
