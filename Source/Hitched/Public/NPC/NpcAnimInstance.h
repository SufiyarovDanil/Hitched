/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NpcAnimInstance.generated.h"


class ANpcBase;

/**
 * 
 */
UCLASS()
class HITCHED_API UNpcAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner")
	ANpcBase* CharacterOwner = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn movement")
	float MaxSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn movement")
	float CurrentSpeed;

public:

	UNpcAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:

	virtual void NativeBeginPlay() override;

};
