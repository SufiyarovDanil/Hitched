/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "NPC/NpcAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NPC/NpcBase.h"


UNpcAnimInstance::UNpcAnimInstance()
{
	MaxSpeed = 1.f;
	CurrentSpeed = 0.f;
}


void UNpcAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	CharacterOwner = Cast<ANpcBase>(TryGetPawnOwner());

	if (!CharacterOwner)
	{
		return;
	}

	MaxSpeed = CharacterOwner
		->GetCharacterMovement()
		->MaxWalkSpeed;
}


void UNpcAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (!CharacterOwner)
	{
		return;
	}

	CurrentSpeed = CharacterOwner
		->GetCharacterMovement()
		->Velocity
		.Size();
}
