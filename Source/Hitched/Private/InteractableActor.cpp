/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "InteractableActor.h"


// Sets default values
AInteractableActor::AInteractableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActionType = EActionType::Interacting;
}


// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AInteractableActor::DoAction_Implementation(AActor* Caller)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, FString::Printf(TEXT("Interacting")));
	}
}


void AInteractableActor::StartFocus_Implementation()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, FString::Printf(TEXT("Focusing on Interactable object")));
	}
}


void AInteractableActor::EndFocus_Implementation()
{
	Super::EndFocus_Implementation();
}
