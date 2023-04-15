/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "Vincent/Inventory/PickableActor.h"
#include "Vincent/VincentBloodberry.h"
#include "Vincent/Inventory/VincentInventoryComponent.h"


APickableActor::APickableActor()
{
	PickableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	PickableMesh->bReceivesDecals = false;
	PickableMesh->CastShadow = false;
	PickableMesh->SetCollisionObjectType(ECC_WorldDynamic);
	PickableMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	PickableMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	PickableMesh->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
	RootComponent = PickableMesh;

	bIsPicked = false;
	bIsPickedUpOnce = false;
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
	if (bIsPickedUpOnce)
	{
		return;
	}

	PickingCharacter = Cast<AVincentBloodberry>(Caller);

	if (!PickingCharacter)
	{
		return;
	}

	PickableMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	PickStartLocation = GetActorLocation();
	PickTimeProgress = 0.f;
	bIsPicked = true;
	bIsPickedUpOnce = true;
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
