/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "Weapons/KrugerMK1.h"

AKrugerMK1::AKrugerMK1()
{
	FWeaponData NewWeaponConfig;
	NewWeaponConfig.bInfiniteAmmo = false;
	NewWeaponConfig.bInfiniteClip = false;
	NewWeaponConfig.MaxAmmo = 28;
	NewWeaponConfig.AmmoPerClip = 7;
	NewWeaponConfig.InitialClips = 4;
	NewWeaponConfig.TimeBetweenShots = 0.5f;
	NewWeaponConfig.NoAnimReloadDuration = 1.0f;

	UpdateConfig(NewWeaponConfig);

	// static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMeshAsset(
	// 	TEXT("StaticMesh'/Game/Environment/Meshes/KrugerMK1.KrugerMK1'"));

	// if (WeaponMeshAsset.Succeeded())
	// {
	// 	WeaponMesh->SetStaticMesh(WeaponMeshAsset.Object);
	// }

	// static ConstructorHelpers::FObjectFinder<UAnimMontage> EquipAnimAsset(
	// 	TEXT("AnimMontage'/Game/Vincent/Animations/TEST/RightHandDrawPistol_Montage.RightHandDrawPistol_Montage'"));

	// if (EquipAnimAsset.Succeeded())
	// {
	// 	EquipAnim = EquipAnimAsset.Object;
	// }

	// static ConstructorHelpers::FObjectFinder<UAnimMontage> UnequipAnimAsset(
	// 	TEXT("AnimMontage'/Game/Vincent/Animations/TEST/RightHandHidePistol_Montage.RightHandHidePistol_Montage'"));

	// if (UnequipAnimAsset.Succeeded())
	// {
	// 	UnequipAnim = UnequipAnimAsset.Object;
	// }

	// static ConstructorHelpers::FObjectFinder<UAnimMontage> FireAnimAsset(
	// 	TEXT("AnimMontage'/Game/Vincent/Animations/TEST/RightHandShootPistol_Montage.RightHandShootPistol_Montage'"));

	// if (FireAnimAsset.Succeeded())
	// {
	// 	FireAnim = FireAnimAsset.Object;
	// }

	// static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadAnimAsset(
	// 	TEXT("AnimMontage'/Game/Vincent/Animations/TEST/RightHandReloadPistol_Montage.RightHandReloadPistol_Montage'"));

	// if (ReloadAnimAsset.Succeeded())
	// {
	// 	ReloadAnim = ReloadAnimAsset.Object;
	// }
}
