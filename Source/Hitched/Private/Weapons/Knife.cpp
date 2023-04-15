/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "Weapons/Knife.h"
#include "Vincent/VincentBloodberry.h"
#include "Vincent/Inventory/VincentInventoryComponent.h"


AKnife::AKnife()
{
	FWeaponData NewWeaponConfig;
	NewWeaponConfig.bInfiniteAmmo = true;
	NewWeaponConfig.bInfiniteClip = true;
	NewWeaponConfig.MaxAmmo = 1;
	NewWeaponConfig.AmmoPerClip = 1;
	NewWeaponConfig.InitialClips = 1;
	NewWeaponConfig.TimeBetweenShots = 0.4f;
	NewWeaponConfig.ReloadDuration = 0.f;
	NewWeaponConfig.EquipDuration = 1.f;

	UpdateConfig(NewWeaponConfig);

	WeaponType = EWeapons::Melee;

	AttachWeaponRelativeLocation = FVector(-0.033177f, -0.101979f, -0.021260f);
	AttachWeaponRelativeRotation = FRotator(-9.846377f, -72.501816f, -61.519501f);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMeshAsset(
		TEXT("StaticMesh'/Game/Environment/Meshes/Knife.Knife'"));

	if (WeaponMeshAsset.Succeeded())
	{
		PickableMesh->SetStaticMesh(WeaponMeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EquipAnimAsset(
		TEXT("AnimMontage'/Game/Vincent/Animations/RightHandDrawKnife_Montage.RightHandDrawKnife_Montage'"));

	if (EquipAnimAsset.Succeeded())
	{
		EquipAnim = EquipAnimAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> UnequipAnimAsset(
		TEXT("AnimMontage'/Game/Vincent/Animations/RightHandHideKnife_Montage.RightHandHideKnife_Montage'"));

	if (UnequipAnimAsset.Succeeded())
	{
		UnequipAnim = UnequipAnimAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireAnimAsset(
		TEXT("AnimMontage'/Game/Vincent/Animations/RightHandAttackKnife_Montage.RightHandAttackKnife_Montage'"));

	if (FireAnimAsset.Succeeded())
	{
		FireAnim = FireAnimAsset.Object;
	}
}


void AKnife::OnAddingToInventory()
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
		if (AKnife* Knife = Cast<AKnife>(Item))
		{
			return;
		}
	}

	PickingCharacter
		->GetInventoryComponent()
		->AddItem(this);
	
	Super::OnAddingToInventory();
}