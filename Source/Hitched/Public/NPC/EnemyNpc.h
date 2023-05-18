/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#pragma once

#include "CoreMinimal.h"
#include "NPC/NpcBase.h"
#include "EnemyNpc.generated.h"


/**
 * 
 */
UCLASS()
class HITCHED_API AEnemyNpc : public ANpcBase
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(VisibleAnywhere, Category = "Animation")
	UAnimMontage* AttackAnimMontage = nullptr;

public:

	/* Sets default values */
	AEnemyNpc();

	void Attack();

};
