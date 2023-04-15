/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase.h"
#include "Knife.generated.h"

/**
 * 
 */
UCLASS()
class HITCHED_API AKnife : public AWeaponBase
{
	GENERATED_BODY()
	
public:

	AKnife();

protected:

	virtual void OnAddingToInventory() override;

};
