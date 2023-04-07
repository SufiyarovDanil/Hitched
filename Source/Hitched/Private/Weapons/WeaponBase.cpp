/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "Weapons/WeaponBase.h"
#include "Vincent/VincentBloodberry.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->bReceivesDecals = false;
	WeaponMesh->CastShadow = false;
	WeaponMesh->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	RootComponent = WeaponMesh;

	CurrentWeaponState = EWeaponState::Idle;
}


// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AWeaponBase::StartFire()
{
	if (CurrentWeaponState != EWeaponState::Idle)
	{
		return;
	}

	PlayWeaponAnimation(FireAnim);
	GetWorldTimerManager().SetTimer(FiringCooldown, this, &AWeaponBase::OnEndFire, WeaponConfig.TimeBetweenShots, false);
}


void AWeaponBase::Reload()
{
	// ...
}


void AWeaponBase::Equip()
{
	// ...
}


void AWeaponBase::Unequip()
{
	// ...
}


void AWeaponBase::OnEndFire()
{
	CurrentWeaponState = EWeaponState::Idle;
}


void AWeaponBase::OnEndReload()
{
	// ...
}


void AWeaponBase::OnEndEquip()
{
	// ...
}


void AWeaponBase::OnEndUnequip()
{
	// ...
}


void AWeaponBase::SetWeaponState(EWeaponState NewState)
{
	CurrentWeaponState = NewState;
}


void AWeaponBase::UpdateConfig(FWeaponData NewData)
{
	WeaponConfig = NewData;
}


void AWeaponBase::PlayWeaponSound(USoundCue* Sound)
{
	if (Sound && OwningCharacter)
	{
		//UGameplayStatics::SpawnSoundAttached(Sound, OwningCharacter->GetRootComponent());
	}
}


void AWeaponBase::PlayWeaponAnimation(UAnimMontage* const Animation)
{
	if (Animation && OwningCharacter)
	{
		OwningCharacter->PlayAnimMontage(Animation);
	}
}


void AWeaponBase::StopWeaponAnimation(UAnimMontage* const Animation)
{
	// ...
}
