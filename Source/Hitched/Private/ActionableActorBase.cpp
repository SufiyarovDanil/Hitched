/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "ActionableActorBase.h"


// Sets default values
AActionableActorBase::AActionableActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActionType = EActionType::Interacting;
}


// Called when the game starts or when spawned
void AActionableActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AActionableActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AActionableActorBase::DoAction_Implementation(AActor* Caller)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, FString::Printf(TEXT("I have no idea...")));
	}
}


void AActionableActorBase::StartFocus_Implementation()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, FString::Printf(TEXT("Focusing")));
	}
}


void AActionableActorBase::EndFocus_Implementation()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, FString::Printf(TEXT("Ending focus")));
	}
}
