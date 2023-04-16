/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "Components/FootstepComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Vincent/VincentBloodberry.h"


// Sets default values for this component's properties
UFootstepComponent::UFootstepComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<USoundCue> MetalLoudFSAsset(
		TEXT("SoundCue'/Game/Sounds/Footsteps/MetalLoudFS.MetalLoudFS'"));

	static ConstructorHelpers::FObjectFinder<USoundCue> MetalQuietFSAsset(
		TEXT("SoundCue'/Game/Sounds/Footsteps/MetalQuietFS.MetalQuietFS'"));

	static ConstructorHelpers::FObjectFinder<USoundCue> MetalJumpAsset(
		TEXT("SoundCue'/Game/Sounds/Footsteps/MetalJ.MetalJ'"));

	static ConstructorHelpers::FObjectFinder<USoundCue> RockLoudFSAsset(
		TEXT("SoundCue'/Game/Sounds/Footsteps/RockLoudFS.RockLoudFS'"));
	
	static ConstructorHelpers::FObjectFinder<USoundCue> RockQuietFSAsset(
		TEXT("SoundCue'/Game/Sounds/Footsteps/RockQuietFS.RockQuietFS'"));
	
	static ConstructorHelpers::FObjectFinder<USoundCue> RockJumpAsset(
		TEXT("SoundCue'/Game/Sounds/Footsteps/RockJ.RockJ'"));

	static ConstructorHelpers::FObjectFinder<USoundCue> TileLoudFSAsset(
		TEXT("SoundCue'/Game/Sounds/Footsteps/TileLoudFS.TileLoudFS'"));
	
	static ConstructorHelpers::FObjectFinder<USoundCue> TileQuietFSAsset(
		TEXT("SoundCue'/Game/Sounds/Footsteps/TileQuietFS.TileQuietFS'"));
	
	static ConstructorHelpers::FObjectFinder<USoundCue> TileJumpAsset(
		TEXT("SoundCue'/Game/Sounds/Footsteps/TileJ.TileJ'"));

	static ConstructorHelpers::FObjectFinder<USoundCue> WoodLoudFSAsset(
		TEXT("SoundCue'/Game/Sounds/Footsteps/WoodLoudFS.WoodLoudFS'"));

	static ConstructorHelpers::FObjectFinder<USoundCue> WoodQuietFSAsset(
		TEXT("SoundCue'/Game/Sounds/Footsteps/WoodQuietFS.WoodQuietFS'"));

	static ConstructorHelpers::FObjectFinder<USoundCue> WoodJumpAsset(
		TEXT("SoundCue'/Game/Sounds/Footsteps/WoodJ.WoodJ'"));

	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> MetalPhysmatAsset(
		TEXT("PhysicalMaterial'/Game/Environment/PhysMaterials/Metal_Physmat.Metal_Physmat'"));

	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> RockPhysmatAsset(
		TEXT("PhysicalMaterial'/Game/Environment/PhysMaterials/Rock_Physmat.Rock_Physmat'"));

	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> TilePhysmatAsset(
		TEXT("PhysicalMaterial'/Game/Environment/PhysMaterials/Tile_Physmat.Tile_Physmat'"));

	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> WoodPhysmatAsset(
		TEXT("PhysicalMaterial'/Game/Environment/PhysMaterials/Wood_Physmat.Wood_Physmat'"));

	// Init FS Loud SFX
	if (MetalLoudFSAsset.Succeeded() && MetalPhysmatAsset.Succeeded())
	{
		FootstepLoudSfxDataMap.Add(MetalPhysmatAsset.Object, MetalLoudFSAsset.Object);
	}

	if (RockLoudFSAsset.Succeeded() && RockPhysmatAsset.Succeeded())
	{
		FootstepLoudSfxDataMap.Add(RockPhysmatAsset.Object, RockLoudFSAsset.Object);
	}

	if (TileLoudFSAsset.Succeeded() && TilePhysmatAsset.Succeeded())
	{
		FootstepLoudSfxDataMap.Add(TilePhysmatAsset.Object, TileLoudFSAsset.Object);
	}

	if (WoodLoudFSAsset.Succeeded() && WoodPhysmatAsset.Succeeded())
	{
		FootstepLoudSfxDataMap.Add(WoodPhysmatAsset.Object, WoodLoudFSAsset.Object);
	}

	// Init FS Quiet SFX
	if (MetalQuietFSAsset.Succeeded() && MetalPhysmatAsset.Succeeded())
	{
		FootstepQuietSfxDataMap.Add(MetalPhysmatAsset.Object, MetalQuietFSAsset.Object);
	}

	if (RockQuietFSAsset.Succeeded() && RockPhysmatAsset.Succeeded())
	{
		FootstepQuietSfxDataMap.Add(RockPhysmatAsset.Object, RockQuietFSAsset.Object);
	}

	if (TileQuietFSAsset.Succeeded() && TilePhysmatAsset.Succeeded())
	{
		FootstepQuietSfxDataMap.Add(TilePhysmatAsset.Object, TileQuietFSAsset.Object);
	}

	if (WoodQuietFSAsset.Succeeded() && WoodPhysmatAsset.Succeeded())
	{
		FootstepQuietSfxDataMap.Add(WoodPhysmatAsset.Object, WoodQuietFSAsset.Object);
	}

	// Init Jump SFX
	if (MetalJumpAsset.Succeeded() && MetalPhysmatAsset.Succeeded())
	{
		JumpSfxDataMap.Add(MetalPhysmatAsset.Object, MetalJumpAsset.Object);
	}

	if (RockJumpAsset.Succeeded() && RockPhysmatAsset.Succeeded())
	{
		JumpSfxDataMap.Add(RockPhysmatAsset.Object, RockJumpAsset.Object);
	}

	if (TileJumpAsset.Succeeded() && TilePhysmatAsset.Succeeded())
	{
		JumpSfxDataMap.Add(TilePhysmatAsset.Object, TileJumpAsset.Object);
	}

	if (WoodJumpAsset.Succeeded() && WoodPhysmatAsset.Succeeded())
	{
		JumpSfxDataMap.Add(WoodPhysmatAsset.Object, WoodJumpAsset.Object);
	}
}


// Called when the game starts
void UFootstepComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<ACharacter>(GetOwner());
}


// Called every frame
void UFootstepComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void UFootstepComponent::MakeFootstep()
{
	if (!OwningCharacter)
	{
		return;
	}

	UPhysicalMaterial* PhysMat = FindPhysMatOfFloor();

	if (!PhysMat)
	{
		return;
	}

	if (AVincentBloodberry* Vincent = Cast<AVincentBloodberry>(OwningCharacter))
	{
		if (Vincent->IsRunning())
		{
			DoLoudFootStep(PhysMat);
		}
		else
		{
			DoQuietFootStep(PhysMat);
		}

		return;
	}

	DoLoudFootStep(PhysMat);
}


void UFootstepComponent::OnLanded()
{
	UPhysicalMaterial* PhysMat = FindPhysMatOfFloor();

	if (!PhysMat)
	{
		return;
	}

	DoLand(PhysMat);
}


UPhysicalMaterial* UFootstepComponent::FindPhysMatOfFloor()
{
	if (!OwningCharacter)
	{
		return nullptr;
	}

	const FVector CharLoc = OwningCharacter->GetActorLocation();
	const float CapsuleHalfHeight = OwningCharacter
		->GetCapsuleComponent()
		->GetScaledCapsuleHalfHeight() + 10.f;
	const FVector TraceEnd = FVector(CharLoc.X, CharLoc.Y, CharLoc.Z - CapsuleHalfHeight);

	FHitResult Hit;
	FCollisionQueryParams CollParams;
	CollParams.AddIgnoredActor(OwningCharacter);
	CollParams.bReturnPhysicalMaterial = true;

	bool b = GetWorld()
		->LineTraceSingleByChannel(
			Hit,
			CharLoc,
			TraceEnd,
			ECollisionChannel::ECC_Visibility,
			CollParams);

	return Hit.PhysMaterial.Get();
}


void UFootstepComponent::PlaySound(USoundCue* Sound)
{
	if (!OwningCharacter || !Sound)
	{
		return;
	}

	UGameplayStatics::SpawnSoundAttached(Sound, OwningCharacter->GetRootComponent());
}


void UFootstepComponent::DoLoudFootStep(UPhysicalMaterial* PhysMat)
{
	if (!OwningCharacter || ! PhysMat)
	{
		return;
	}

	if (!FootstepLoudSfxDataMap.Contains(PhysMat))
	{
		return;
	}

	PlaySound(FootstepLoudSfxDataMap[PhysMat]);
}


void UFootstepComponent::DoQuietFootStep(UPhysicalMaterial* PhysMat)
{
	if (!OwningCharacter || ! PhysMat)
	{
		return;
	}

	if (!FootstepQuietSfxDataMap.Contains(PhysMat))
	{
		return;
	}

	PlaySound(FootstepQuietSfxDataMap[PhysMat]);
}


void UFootstepComponent::DoLand(UPhysicalMaterial* PhysMat)
{
	if (!OwningCharacter || ! PhysMat)
	{
		return;
	}

	if (!JumpSfxDataMap.Contains(PhysMat))
	{
		return;
	}

	PlaySound(JumpSfxDataMap[PhysMat]);
}
