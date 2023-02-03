/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "Vincent/VincentVaultingComponent.h"
#include "Vincent/VincentBloodberry.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"


#define DRAW_DEBUG

// Sets default values for this component's properties
UVincentVaultingComponent::UVincentVaultingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	VaultState = EVaultState::NotVaulting;

	MinVaultingHeight = 0.f;
	MaxVaultingHeight = 0.f;

	Progress = 0.f;

	VaultSpeed = 2.f;
}


// Called when the game starts
void UVincentVaultingComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<AVincentBloodberry>(GetOwner());

	if (OwningCharacter)
	{
		MaxVaultingHeight = OwningCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.f;
		MinVaultingHeight = MaxVaultingHeight / 4.f;
	}
}


// Called every frame
void UVincentVaultingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (VaultState)
	{
	case EVaultState::NotVaulting:
		CanVault();
		break;
	case EVaultState::Vaulting:
		TickVault(DeltaTime);
		break;
	default:
		break;
	}
}


void UVincentVaultingComponent::TickVault(const float DeltaTime)
{
	Progress = FMath::Clamp(Progress + DeltaTime * VaultSpeed, 0.f, 1.f);

	const FVector InterpLoc = FMath::Lerp(StartingLocation, EndingLocation, Progress);

	OwningCharacter->SetActorLocation(InterpLoc, false, nullptr, ETeleportType::TeleportPhysics);

	if (Progress >= 1.0f)
	{
		VaultState = EVaultState::NotVaulting;
	}
}


bool UVincentVaultingComponent::CanVault()
{
	if (IsVaulting())
	{
		return false;
	}

	UCapsuleComponent* CapsuleComp = OwningCharacter->GetCapsuleComponent();

	const FVector VincentWorldLoc = OwningCharacter->GetActorLocation();
	const FVector VaultForwardDistance = OwningCharacter->GetActorForwardVector() * 70.f;

	const float CapsuleHalfHeight = CapsuleComp->GetScaledCapsuleHalfHeight();
	const FVector LineTraceStart = VincentWorldLoc + VaultForwardDistance + FVector(0.f, 0.f, CapsuleHalfHeight);
	const FVector LineTraceEnd = VincentWorldLoc + VaultForwardDistance - FVector(0.f, 0.f, CapsuleHalfHeight);

	FHitResult HitResult;

	FCollisionQueryParams CollParams;
	CollParams.AddIgnoredActor(OwningCharacter);

	if (!GetWorld()->LineTraceSingleByChannel(
		HitResult,
		LineTraceStart,
		LineTraceEnd,
		ECollisionChannel::ECC_Visibility,
		CollParams))
	{

#ifdef DRAW_DEBUG

		DrawDebugLine(GetWorld(), LineTraceStart, LineTraceEnd, FColor::Orange);

#endif

		return false;
	}

#ifdef DRAW_DEBUG

	DrawDebugLine(GetWorld(), LineTraceStart, HitResult.Location, FColor::Green);

#endif

	if (!CanVaultToHit(HitResult))
	{
		return false;
	}

	return true;
}


bool UVincentVaultingComponent::CanVaultToHit(const FHitResult& HitResult)
{
	// Make sure that adjust is in range between min and max vaulting height
	float Adjust = HitResult.Location.Z - HitResult.TraceEnd.Z;

	if (!(Adjust >= MinVaultingHeight && Adjust <= MaxVaultingHeight))
	{
		return false;
	}

	// Make sure that angle of surface is walkable for character
	float FloorAngle = HitResult.Normal.Z;

	if (FloorAngle < OwningCharacter->GetCharacterMovement()->GetWalkableFloorZ())
	{
		return false;
	}

	// Make sure that vault area has enough space for capsule of character
	const float CapsuleHalfHeight = OwningCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const float CapsuleRadius = OwningCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();

	EndingLocation = HitResult.Location + FVector(0.f, 0.f, CapsuleHalfHeight);

	const FVector Center = EndingLocation + FVector(0.f, 0.f, CapsuleRadius);

	if (CheckCapsuleCollision(Center, CapsuleHalfHeight, CapsuleRadius))
	{
		return false;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Emerald, FString::Printf(TEXT("Can vault")));
	}

	return true;
}


bool UVincentVaultingComponent::CheckCapsuleCollision(const FVector Center, const float HalfHeight, const float Radius)
{

#ifdef DRAW_DEBUG

	DrawDebugCapsule
	(
		GetWorld(),
		Center,
		HalfHeight,
		Radius,
		FQuat(FRotator(0.f, 0.f, 0.f)),
		FColor::Emerald
	);

#endif

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);	// means WorldStatic
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);	// means WorldDynamic
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery6);	// means PhysicsBody
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery7);	// means Vehicle
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery8);	// means Destructible

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwningCharacter);

	TArray<AActor*> OutActors;

	return UKismetSystemLibrary::CapsuleOverlapActors
	(
		GetWorld(),
		Center,
		Radius,
		HalfHeight,
		ObjectTypes,
		AActor::StaticClass(),
		ActorsToIgnore,
		OutActors
	);
}


bool UVincentVaultingComponent::IsVaulting()
{
	return VaultState == EVaultState::Vaulting;
}


void UVincentVaultingComponent::Vault()
{
	Progress = 0.f;

	StartingLocation = OwningCharacter->GetActorLocation();

	VaultState = EVaultState::Vaulting;
}
