/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "GrabbableActor.h"


// Sets default values
AGrabbableActor::AGrabbableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActionType = EActionType::Grabbing;
}


// Called when the game starts or when spawned
void AGrabbableActor::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void AGrabbableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AGrabbableActor::DoAction_Implementation(AActor* Caller)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, FString::Printf(TEXT("Grabbing")));
	}
}


void AGrabbableActor::StartFocus_Implementation()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, FString::Printf(TEXT("Focusing on grabbable object")));
	}
}


void AGrabbableActor::EndFocus_Implementation()
{
	Super::EndFocus_Implementation();
}
