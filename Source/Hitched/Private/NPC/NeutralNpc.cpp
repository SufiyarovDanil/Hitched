/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "NPC/NeutralNpc.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


ANeutralNpc::ANeutralNpc()
{

}


void ANeutralNpc::DoAction_Implementation(AActor* Caller)
{
	if (Replicas->GetDuration())
	{

	}

	UGameplayStatics::SpawnSoundAttached(Replicas, RootComponent);
}


void ANeutralNpc::StartFocus_Implementation()
{
	// ...
}


void ANeutralNpc::EndFocus_Implementation()
{
	// ...
}
