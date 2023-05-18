/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "NPC/NeutralNpc.h"
#include "NPC/AI/NeutralAiController.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


ANeutralNpc::ANeutralNpc()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, 270.f, 0.f));
	GetMesh()->SetRelativeScale3D(FVector(0.5f));

    AIControllerClass = ANeutralAiController::StaticClass();

    bIsTalking = false;
}


void ANeutralNpc::BeginPlay()
{
    Super::BeginPlay();

    if (Hairs.Num() > 0)
    {
        const int32 HairIndex = FMath::RandRange(0, Hairs.Num() - 1); 

        HairMesh->SetSkeletalMesh(Hairs[HairIndex]);
    }

    if (TopClothes.Num() > 0)
    {
        const int32 TopClothIndex = FMath::RandRange(0, TopClothes.Num() - 1); 

        TopClothMesh->SetSkeletalMesh(TopClothes[TopClothIndex]);
    }

    if (BottomClothes.Num() > 0)
    {
        const int32 BottomClothIndex = FMath::RandRange(0, BottomClothes.Num() - 1); 

        BottomClothMesh->SetSkeletalMesh(BottomClothes[BottomClothIndex]);
    }
}


void ANeutralNpc::DoAction_Implementation(AActor* Caller)
{
    StartTalking();
    
}


void ANeutralNpc::StartFocus_Implementation()
{
	// ...
}


void ANeutralNpc::EndFocus_Implementation()
{
	// ...
}


void ANeutralNpc::StartTalking()
{
    if (bIsTalking || !Replicas)
    {
        return;
    }

    bIsTalking = true;

    UGameplayStatics::SpawnSoundAttached(Replicas, RootComponent);

    FTimerHandle Coodldown;

	GetWorldTimerManager()
        .SetTimer(Coodldown, this, &ANeutralNpc::EndTalking, Replicas->GetDuration(), false);
}


void ANeutralNpc::EndTalking()
{
    bIsTalking = false;
}
