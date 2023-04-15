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
	SetWeaponState(EWeaponState::Firing);
	GetWorldTimerManager().SetTimer(FiringCooldown, this, &AWeaponBase::OnEndFire, WeaponConfig.TimeBetweenShots, false);
}


void AWeaponBase::Reload()
{
	if (CurrentWeaponState != EWeaponState::Idle)
	{
		return;
	}

	PlayWeaponAnimation(ReloadAnim);
	SetWeaponState(EWeaponState::Reloading);
	GetWorldTimerManager().SetTimer(ReloadWeaponCooldown, this, &AWeaponBase::OnEndReload, WeaponConfig.ReloadDuration, false);
}


void AWeaponBase::Equip()
{
	if (CurrentWeaponState != EWeaponState::Idle)
	{
		return;
	}

	PlayWeaponAnimation(EquipAnim);
	SetWeaponState(EWeaponState::Equipping);
	GetWorldTimerManager().SetTimer(OnEquipFinishedCooldown, this, &AWeaponBase::OnEndEquip, WeaponConfig.EquipDuration, false);
}


void AWeaponBase::Unequip()
{
	if (CurrentWeaponState != EWeaponState::Idle)
	{
		return;
	}

	PlayWeaponAnimation(UnequipAnim);
	SetWeaponState(EWeaponState::Equipping);
	GetWorldTimerManager().SetTimer(OnEquipFinishedCooldown, this, &AWeaponBase::OnEndUnequip, WeaponConfig.EquipDuration, false);
}


void AWeaponBase::OnEndFire()
{
	SetWeaponState(EWeaponState::Idle);
}


void AWeaponBase::OnEndReload()
{
	SetWeaponState(EWeaponState::Idle);
}


void AWeaponBase::OnEndEquip()
{
	SetWeaponState(EWeaponState::Idle);
}


void AWeaponBase::OnEndUnequip()
{
	SetWeaponState(EWeaponState::Idle);
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
	if (Sound && PickingCharacter)
	{
		//UGameplayStatics::SpawnSoundAttached(Sound, PickingCharacter->GetRootComponent());
	}
}


void AWeaponBase::PlayWeaponAnimation(UAnimMontage* const Animation)
{
	if (Animation && PickingCharacter)
	{
		PickingCharacter->PlayAnimMontage(Animation);
	}
}


void AWeaponBase::StopWeaponAnimation(UAnimMontage* const Animation)
{
	if (Animation && PickingCharacter)
	{
		PickingCharacter->StopAnimMontage(Animation);
	}
}
