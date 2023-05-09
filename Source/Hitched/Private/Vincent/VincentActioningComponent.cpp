/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "Vincent/VincentActioningComponent.h"
#include "Vincent/VincentBloodberry.h"
#include "ActionableActorBase.h"


// Sets default values for this component's properties
UVincentActioningComponent::UVincentActioningComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TraceDistance = 150.f;

	bIsGrabbing = false;
	MaxDeflectionDistance = TraceDistance / 2.f;
}


// Called when the game starts
void UVincentActioningComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<AVincentBloodberry>(GetOwner());
}


// Called every frame
void UVincentActioningComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsGrabbing)
	{
		TickGrabbableActor(DeltaTime);

		return;
	}

	TraceForward();
}


void UVincentActioningComponent::TraceForward()
{
	if (!OwningCharacter)
	{
		return;
	}

	FVector Loc;
	FRotator Rot;
	FHitResult Hit;
	FCollisionQueryParams CollParams;

	CollParams.AddIgnoredActor(OwningCharacter);

	OwningCharacter->GetController()->GetPlayerViewPoint(Loc, Rot);

	const FVector Start = Loc;
	const FVector End = Loc + (Rot.Vector() * TraceDistance);

	if (!GetWorld()->LineTraceSingleByChannel
	(
		Hit,
		Start,
		End,
		ECC_MAX,
		CollParams
	))
	{
		// It means that character focusing to nothing and we must end focus to actor
		TryToEndFocus();

		return;
	}

	if (!Hit.GetActor()->Implements<UAction>())
	{
		TryToEndFocus();

		return;
	}

	// if Character switch to new or other interactable actor
	if (FocusedActionableActor != Hit.GetActor())
	{
		TryToEndFocus();
		TryToStartFocus(Hit.GetActor());
	}
}


void UVincentActioningComponent::ActionButtonPressed()
{
	if (!FocusedActionableActor)
	{
		return;
	}
	
	if (AActionableActorBase* ActionableActor = Cast<AActionableActorBase>(FocusedActionableActor))
	{
		switch (ActionableActor->GetActionType())
		{
		case EActionType::Interacting:
			break;
		case EActionType::Grabbing:
			if (bIsGrabbing)
			{
				Drop();
			}
			else
			{
				Grab();
			}
			break;
		default:
			break;
		}
	}

	IAction::Execute_DoAction(FocusedActionableActor, OwningCharacter);
}


void UVincentActioningComponent::TryToEndFocus()
{
	// Character focusing to nothing
	// We must execute end focus function
	if (!FocusedActionableActor)
	{
		return;
	}

	IAction::Execute_EndFocus(FocusedActionableActor);

	FocusedActionableActor = nullptr;
}


void UVincentActioningComponent::TryToStartFocus(AActor* NewActor)
{
	IAction::Execute_StartFocus(NewActor);

	FocusedActionableActor = NewActor;
}


void UVincentActioningComponent::Grab()
{
	bIsGrabbing = true;
}


void UVincentActioningComponent::Drop()
{
	bIsGrabbing = false;
}


void UVincentActioningComponent::TickGrabbableActor(const float DeltaTime)
{
	FVector Loc;
	FRotator Rot;

	OwningCharacter->GetController()->GetPlayerViewPoint(Loc, Rot);

	const FVector ActorLoc = FocusedActionableActor->GetActorLocation();
	const FVector ActorMoveTarget = Loc + (Rot.Vector() * TraceDistance);

	const float DeflectionDistance = FVector::Dist(ActorLoc, ActorMoveTarget);

	if (DeflectionDistance > MaxDeflectionDistance)
	{
		ActionButtonPressed();

		return;
	}

	const FVector InterpActorLoc = FMath::VInterpTo(ActorLoc, ActorMoveTarget, DeltaTime, 10.f);

	FocusedActionableActor->SetActorLocation(InterpActorLoc, true);
}
