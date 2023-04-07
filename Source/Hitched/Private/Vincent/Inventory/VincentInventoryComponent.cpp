/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "Vincent/Inventory/VincentInventoryComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Vincent/VincentBloodberry.h"
#include "Vincent/Inventory/Inventory.h"


// Sets default values for this component's properties
UVincentInventoryComponent::UVincentInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentInventoryState = EInventoryState::Closed;
	TraceForwardLength = 60.f;
	AllowedFloorAngle = 15.f;
	CasePlacingTime = 0.25f;
	CasePlacingProgressTime = 0.f;
	CaseClosingAnimTime = 0.6f;
	CaseClosingAnimProgressTime = 0.f;
}


// Called when the game starts
void UVincentInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<AVincentBloodberry>(GetOwner());
}


// Called every frame
void UVincentInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (CurrentInventoryState)
	{
	case EInventoryState::Opened:
		break;
	case EInventoryState::OnOpening:
		TickPlacingCase(DeltaTime);
		break;
	case EInventoryState::Closed:
		break;
	case EInventoryState::OnClosing:
		TickHidingCase(DeltaTime);
		break;
	default:
		break;
	}
}


void UVincentInventoryComponent::TickPlacingCase(const float DeltaTime)
{
	if (!InventoryCase || !OwningCharacter)
	{
		return;
	}

	const FVector CharacterLocation = OwningCharacter->GetActorLocation();
	const FRotator CharacterRotation = OwningCharacter->GetActorRotation();
	const FRotator CasePlaceRotation = FRotator(CharacterRotation.Pitch, CharacterRotation.Yaw + 90.f, CharacterRotation.Roll + 90.f);
	const FVector NewLocation = FMath::Lerp(CharacterLocation, CasePlaceLocation, CasePlacingProgressTime * (1.f / CasePlacingTime));
	const FRotator NewRotation = FMath::Lerp(CharacterRotation, CasePlaceRotation, CasePlacingProgressTime * (1.f / CasePlacingTime));

	InventoryCase->SetActorLocationAndRotation(NewLocation, NewRotation);

	if (CasePlacingProgressTime >= CasePlacingTime)
	{
		CurrentInventoryState = EInventoryState::Opened;
		InventoryCase->OpenCase();
	}

	CasePlacingProgressTime = FMath::Clamp(CasePlacingProgressTime + DeltaTime, 0.f, CasePlacingTime);
}


void UVincentInventoryComponent::TickHidingCase(const float DeltaTime)
{
	if (!InventoryCase)
	{
		return;
	}

	if (CaseClosingAnimProgressTime <= CaseClosingAnimTime)
    {
		CaseClosingAnimProgressTime += DeltaTime;
		return;
	}

	const FVector CharacterLocation = OwningCharacter->GetActorLocation();
	const FRotator CharacterRotation = OwningCharacter->GetActorRotation();
	const FRotator CasePlaceRotation = FRotator(CharacterRotation.Pitch, CharacterRotation.Yaw + 90.f, CharacterRotation.Roll + 90.f);
	const FVector NewLocation = FMath::Lerp(CasePlaceLocation, CharacterLocation, CasePlacingProgressTime * (1.f / CasePlacingTime));
	const FRotator NewRotation = FMath::Lerp(CasePlaceRotation, CharacterRotation, CasePlacingProgressTime * (1.f / CasePlacingTime));

	InventoryCase->SetActorLocationAndRotation(NewLocation, NewRotation);

	if (CasePlacingProgressTime >= CasePlacingTime)
	{
		CurrentInventoryState = EInventoryState::Closed;
		GetWorld()->DestroyActor(InventoryCase);
		InventoryCase = nullptr;
	}

	CasePlacingProgressTime = FMath::Clamp(CasePlacingProgressTime + DeltaTime, 0.f, CasePlacingTime);
}


void UVincentInventoryComponent::PlaceCase()
{
	if (CurrentInventoryState != EInventoryState::Closed)
	{
		return;
	}

	if (!CanPlaceCase())
	{
		return;
	}

	CasePlacingProgressTime = 0.f;
	InventoryCase = GetWorld()->SpawnActor<AInventory>();
	CurrentInventoryState = EInventoryState::OnOpening;
}


void UVincentInventoryComponent::HideCase()
{
	if (CurrentInventoryState != EInventoryState::Opened)
	{
		return;
	}

	CasePlacingProgressTime = 0.f;
	CaseClosingAnimProgressTime = 0.f;
	InventoryCase->CloseCase();
	CurrentInventoryState = EInventoryState::OnClosing;
}


void UVincentInventoryComponent::ToggleButtonPressed()
{
	if (CurrentInventoryState == EInventoryState::Closed)
	{
		PlaceCase();
	}
	else if (CurrentInventoryState == EInventoryState::Opened)
	{
		HideCase();
	}
}


bool UVincentInventoryComponent::CanPlaceCase()
{
	UCapsuleComponent* CharacterCapsule = OwningCharacter->GetCapsuleComponent();
	const float CapsuleHalfHeight = CharacterCapsule->GetScaledCapsuleHalfHeight();

	const FVector CharacterForwardDistance = OwningCharacter->GetActorForwardVector() * TraceForwardLength;
	const FVector CharacterLocation = OwningCharacter->GetActorLocation();
	const FRotator CharacterRotation = OwningCharacter->GetActorRotation();
	const FVector LineTraceForwardEnd = CharacterLocation + CharacterForwardDistance;
	const FVector LineTraceonFloorEnd = LineTraceForwardEnd - FVector(0.f, 0.f, CapsuleHalfHeight + 10.f); // adding 10.f for floor checking

	FHitResult FirstHR;
	FHitResult SecondHR;
	FCollisionQueryParams CollParams;
	CollParams.AddIgnoredActor(OwningCharacter);

	FCollisionShape CaseCollision = FCollisionShape::MakeBox(FVector(13.5f, 23.f, 5.5f)); // Hardcoded af

	/* Trace from forward to floor */
	if (!GetWorld()->SweepSingleByChannel(
		FirstHR,
		LineTraceForwardEnd,
		LineTraceonFloorEnd,
		FQuat(CharacterRotation),
		ECC_Visibility,
		CaseCollision,
		CollParams))
	{
		return false;
	}

	//DrawDebugBox(GetWorld(), FirstHR.Location, FVector(13.5f, 23.f, 5.5f), FQuat(CharacterRotation), FColor::Emerald, false, 2.f, 0u, 1.f);

	/* Trace on Floor */
	if (GetWorld()->LineTraceSingleByChannel(
		SecondHR,
		CharacterLocation,
		FirstHR.Location,
		ECollisionChannel::ECC_Visibility,
		CollParams))
	{
		return false;
	}

	if (!CanPlaceToHit(FirstHR, CaseCollision))
	{
		return false;
	}

	CasePlaceLocation = FirstHR.Location;

	return true;
}

bool UVincentInventoryComponent::CanPlaceToHit(FHitResult& Hit, FCollisionShape& CaseCollision)
{
	if (!Hit.GetActor() || Hit.GetActor()->GetRootComponent()->IsSimulatingPhysics())
	{
		return false;
	}

	const float FloorAngle = (1.f - Hit.Normal.Z) * 90.f;

	if (FloorAngle > AllowedFloorAngle)
	{
		return false;
	}

	if (Hit.Distance == 0.f)
	{
		return false;
	}

	return true;
}
