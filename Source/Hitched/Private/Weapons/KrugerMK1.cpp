/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "Weapons/KrugerMK1.h"
#include "Vincent/VincentBloodberry.h"
#include "Vincent/Inventory/VincentInventoryComponent.h"


AKrugerMK1::AKrugerMK1()
{
	FWeaponData NewWeaponConfig;
	NewWeaponConfig.bInfiniteAmmo = false;
	NewWeaponConfig.bInfiniteClip = false;
	NewWeaponConfig.MaxAmmo = 28;
	NewWeaponConfig.AmmoPerClip = 7;
	NewWeaponConfig.InitialClips = 4;
	NewWeaponConfig.TimeBetweenShots = 0.24f;
	NewWeaponConfig.ReloadDuration = 1.f;
	NewWeaponConfig.EquipDuration = 1.f;

	UpdateConfig(NewWeaponConfig);

	WeaponType = EWeapons::Pistol;

	AttachWeaponRelativeLocation = FVector(0.019334f, -0.241562f, -0.058301f);
	AttachWeaponRelativeRotation = FRotator(-15.682512f, -73.985100f, -68.987625f);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMeshAsset(
		TEXT("StaticMesh'/Game/Environment/Meshes/KrugerMK1.KrugerMK1'"));

	if (WeaponMeshAsset.Succeeded())
	{
		PickableMesh->SetStaticMesh(WeaponMeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EquipAnimAsset(
		TEXT("AnimMontage'/Game/Vincent/Animations/RightHandDrawPistol_Montage.RightHandDrawPistol_Montage'"));

	if (EquipAnimAsset.Succeeded())
	{
		EquipAnim = EquipAnimAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> UnequipAnimAsset(
		TEXT("AnimMontage'/Game/Vincent/Animations/RightHandHidePistol_Montage.RightHandHidePistol_Montage'"));

	if (UnequipAnimAsset.Succeeded())
	{
		UnequipAnim = UnequipAnimAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireAnimAsset(
		TEXT("AnimMontage'/Game/Vincent/Animations/RightHandShootPistol_Montage.RightHandShootPistol_Montage'"));

	if (FireAnimAsset.Succeeded())
	{
		FireAnim = FireAnimAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadAnimAsset(
		TEXT("AnimMontage'/Game/Vincent/Animations/RightHandReloadPistol_Montage.RightHandReloadPistol_Montage'"));

	if (ReloadAnimAsset.Succeeded())
	{
		ReloadAnim = ReloadAnimAsset.Object;
	}
}


void AKrugerMK1::OnAddingToInventory()
{
	if (!PickingCharacter)
	{
		return;
	}

	TArray<AWeaponBase*> Storage = PickingCharacter
		->GetInventoryComponent()
		->GetInventoryStorage();

	for (AWeaponBase* const Item : Storage)
	{
		if (AKrugerMK1* Kruger = Cast<AKrugerMK1>(Item))
		{
			return;
		}
	}

	PickingCharacter
		->GetInventoryComponent()
		->AddItem(this);
	
	Super::OnAddingToInventory();
}
