/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"


class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class AEnemyNpc;


UENUM(BlueprintType)
enum class ENpcState : uint8
{
	Calm	UMETA(DisplayName = "Calm"),
	Caution	UMETA(DisplayName = "Caution"),
	Battle	UMETA(DisplayName = "Battle")
};


/**
 * 
 */
UCLASS()
class HITCHED_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, Category = "Ai")
	UAIPerceptionComponent* AiPerception = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Ai")
	UAISenseConfig_Sight* AiSight = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Character")
	AEnemyNpc* OwningCharacter = nullptr;

	ENpcState CurrentState;

	float SightLevel;

	float SightFillSpeed;

private:

	void TickCalmState(float DeltaTime);

	void TickCautionState(float DeltaTime);

	void TickBattleState(float DeltaTime);

	UFUNCTION()
	void OnPerceptionUpdatedDelegate(AActor* Actor, FAIStimulus Stimulus);

	bool IsPlayerVisible();

	bool CheckVisiblity(FVector VinceLoc, const float LightLevel);

protected:

	virtual void BeginPlay() override;

public:

	AEnemyAIController();

	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;

};
