/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "Vincent/VincentInteractingComponent.h"
#include "Vincent/VincentBloodberry.h"
#include "DrawDebugHelpers.h"
#include "Interact.h"

// Sets default values for this component's properties
UVincentInteractingComponent::UVincentInteractingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TraceDistance = 100.f;
}


// Called when the game starts
void UVincentInteractingComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<AVincentBloodberry>(GetOwner());
}


// Called every frame
void UVincentInteractingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TraceForward();
}


void UVincentInteractingComponent::TraceForward()
{
	if (!OwningCharacter)
	{
		return;
	}

	FVector Loc;
	FRotator Rot;
	FHitResult Hit;

	OwningCharacter->GetController()->GetPlayerViewPoint(Loc, Rot);

	const FVector Start = Loc;
	const FVector End = Loc + (Rot.Vector() * TraceDistance);

	GetWorld()->LineTraceSingleByChannel
	(
		Hit,
		Start,
		End,
		ECC_Visibility
	);

	AActor* NewFocusedInteractableActor = Hit.GetActor();

	if (!NewFocusedInteractableActor)
	{
		TryToEndFocus();

		return;
	}

	IInteract* Interface = Cast<IInteract>(NewFocusedInteractableActor);

	if (!Interface)
	{
		// It means that character focusing to nothing and we must end focus to actor
		TryToEndFocus();

		return;
	}

	// if Character switch to new or other interactable actor
	if (FocusedInteractableActor != NewFocusedInteractableActor)
	{
		TryToStartFocus(NewFocusedInteractableActor);
	}
}


void UVincentInteractingComponent::InteractButtonPressed()
{
	/*FVector Loc;
	FRotator Rot;
	FHitResult Hit;

	OwningCharacter->GetController()->GetPlayerViewPoint(Loc, Rot);

	const FVector Start = Loc;
	const FVector End = Loc + (Rot.Vector() * TraceDistance);

	DrawDebugLine(GetWorld(), Start, End, FColor::Orange, false, 5.f);*/

	if (!FocusedInteractableActor)
	{
		return;
	}

	IInteract::Execute_Interact(FocusedInteractableActor, OwningCharacter);
}


void UVincentInteractingComponent::TryToEndFocus()
{
	// Character focusing to nothing
	// We must execute end focus function
	if (!FocusedInteractableActor)
	{
		return;
	}

	IInteract::Execute_EndFocus(FocusedInteractableActor);
	FocusedInteractableActor = nullptr;
}


void UVincentInteractingComponent::TryToStartFocus(AActor* NewActor)
{
	if (!Cast<IInteract>(NewActor))
	{
		return;
	}

	IInteract::Execute_StartFocus(NewActor);
	FocusedInteractableActor = NewActor;
}
