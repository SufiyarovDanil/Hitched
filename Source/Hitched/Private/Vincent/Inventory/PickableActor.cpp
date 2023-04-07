/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "Vincent/Inventory/PickableActor.h"
#include "Vincent/VincentBloodberry.h"


APickableActor::APickableActor()
{
	bIsPicked = false;
	PickTime = 0.2f;
	PickTimeProgress = 0.f;
}


void APickableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPicked)
	{
		TickMagnetisingToCharacter(DeltaTime);
	}
}


void APickableActor::BeginPlay()
{
	Super::BeginPlay();

}


void APickableActor::DoAction_Implementation(AActor* Caller)
{
	if (bIsPicked)
	{
		return;
	}

	PickingCharacter = Cast<AVincentBloodberry>(Caller);

	if (!PickingCharacter)
	{
		return;
	}

	PickStartLocation = GetActorLocation();
	PickTimeProgress = 0.f;
	bIsPicked = true;
}


void APickableActor::StartFocus_Implementation()
{
	Super::StartFocus_Implementation();
}


void APickableActor::EndFocus_Implementation()
{
	Super::EndFocus_Implementation();
}


void APickableActor::TickMagnetisingToCharacter(const float DeltaTime)
{
	if (!PickingCharacter)
	{
		return;
	}

	const FVector CharacterLocation = PickingCharacter->GetActorLocation();
	const FVector NewLocation = FMath::Lerp(PickStartLocation, CharacterLocation, PickTimeProgress * (1.f / PickTime));

	SetActorLocation(NewLocation);

	if (PickTimeProgress >= PickTime)
	{
		bIsPicked = false;
		AddToInventory();
	}

	PickTimeProgress += DeltaTime;
}

void APickableActor::AddToInventory()
{
	OnAddingToInventory();
}

void APickableActor::OnAddingToInventory()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, FString::Printf(TEXT("This item added to inventory")));
	}
}
