/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "Vincent/Inventory/VincentInventoryComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapons/WeaponBase.h"
#include "Vincent/VincentBloodberry.h"
#include "Vincent/Inventory/Inventory.h"
#include "Vincent/Inventory/PickableActor.h"


// Sets default values for this component's properties
UVincentInventoryComponent::UVincentInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentItemIndex = 0;
	CurrentInventoryState = EInventoryState::Closed;
	TraceForwardLength = 60.f;
	AllowedFloorAngle = 15.f;
	CasePlacingTime = 0.25f;
	CasePlacingProgressTime = 0.f;
	CaseClosingAnimTime = 0.6f;
	CaseClosingAnimProgressTime = 0.f;
	TheAssOfTheWorld = FVector(0.f, 0.f, -100000.f);
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

	const FVector PlaceStartLocation = OwningCharacter->GetActorLocation() - (OwningCharacter->GetActorRightVector() * TraceForwardLength);
	const FRotator CharacterRotation = OwningCharacter->GetActorRotation();
	const FRotator CasePlaceRotation = FRotator(CharacterRotation.Pitch, CharacterRotation.Yaw + 90.f, CharacterRotation.Roll + 90.f);
	const FVector NewLocation = FMath::Lerp(PlaceStartLocation, CasePlaceLocation, CasePlacingProgressTime * (1.f / CasePlacingTime));
	const FRotator NewRotation = FMath::Lerp(CharacterRotation, CasePlaceRotation, CasePlacingProgressTime * (1.f / CasePlacingTime));

	InventoryCase->SetActorLocationAndRotation(NewLocation, NewRotation);

	if (CasePlacingProgressTime >= CasePlacingTime)
	{
		CurrentInventoryState = EInventoryState::Opened;
		InventoryCase->OpenCase();
		CurrentItemIndex = 0;

		MoveItemToInventoryCase(CurrentItemIndex);
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

	const FVector PlaceEndLocation = OwningCharacter->GetActorLocation() - (OwningCharacter->GetActorRightVector() * TraceForwardLength);
	const FRotator CharacterRotation = OwningCharacter->GetActorRotation();
	const FRotator CasePlaceRotation = FRotator(CharacterRotation.Pitch, CharacterRotation.Yaw + 90.f, CharacterRotation.Roll + 90.f);
	const FVector NewLocation = FMath::Lerp(CasePlaceLocation, PlaceEndLocation, CasePlacingProgressTime * (1.f / CasePlacingTime));
	const FRotator NewRotation = FMath::Lerp(CasePlaceRotation, CharacterRotation, CasePlacingProgressTime * (1.f / CasePlacingTime));

	InventoryCase->SetActorLocationAndRotation(NewLocation, NewRotation);

	if (InventoryStorage.Num() > 0
		&& CurrentItemIndex >= 0 && CurrentItemIndex < InventoryStorage.Num())
	{
		InventoryStorage[CurrentItemIndex]->SetActorLocation(TheAssOfTheWorld);
	}

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

	FHitResult HitResult;

	if (!CanPlaceCase(HitResult))
	{
		return;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Inventory case %d"), InventoryStorage.Num()));
	}

	CasePlaceLocation = HitResult.Location;
	CasePlacingProgressTime = 0.f;
	InventoryCase = GetWorld()->SpawnActor<AInventory>();
	CurrentItemIndex = 0;
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


void UVincentInventoryComponent::MoveItemToInventoryCase(int32 ItemIndex)
{
	if ((ItemIndex < 0 || ItemIndex > (InventoryStorage.Num()-1))
		|| !InventoryCase)
	{
		return;
	}

	InventoryStorage[ItemIndex]
		->SetActorLocationAndRotation(
			InventoryCase->GetActorLocation(),
			InventoryCase->GetActorRotation());
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


void UVincentInventoryComponent::NextItem()
{
	if (InventoryStorage.Num() == 0
		|| CurrentInventoryState != EInventoryState::Opened)
	{
		return;
	}

	InventoryStorage[CurrentItemIndex]->SetActorLocation(TheAssOfTheWorld);
	CurrentItemIndex = (CurrentItemIndex + 1) % InventoryStorage.Num();

	MoveItemToInventoryCase(CurrentItemIndex);
}


void UVincentInventoryComponent::PrevItem()
{
	if (InventoryStorage.Num() == 0
		|| CurrentInventoryState != EInventoryState::Opened)
	{
		return;
	}

	InventoryStorage[CurrentItemIndex]->SetActorLocation(TheAssOfTheWorld);
	CurrentItemIndex = FMath::Abs((CurrentItemIndex - 1) % InventoryStorage.Num());

	MoveItemToInventoryCase(CurrentItemIndex);
}


void UVincentInventoryComponent::AddItem(AWeaponBase* Weapon)
{
	if (!Weapon)
	{
		return;
	}

	InventoryStorage.AddUnique(Weapon);

	Weapon->SetActorLocation(TheAssOfTheWorld);
}


bool UVincentInventoryComponent::CanPlaceCase(FHitResult& Hit)
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

	Hit = FirstHR;

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


AWeaponBase* UVincentInventoryComponent::TakeSelectedItem()
{
	if (CurrentItemIndex < 0 || CurrentItemIndex >= InventoryStorage.Num()
		|| CurrentInventoryState != EInventoryState::Opened)
	{
		return nullptr;
	}

	AWeaponBase* TakenWeapon = InventoryStorage[CurrentItemIndex];
	InventoryStorage.RemoveAt(CurrentItemIndex);
	CurrentItemIndex = 0;

	HideCase();

	return TakenWeapon;
}
