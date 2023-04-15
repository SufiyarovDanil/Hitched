/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#pragma once

#include "CoreMinimal.h"
#include "Vincent/Inventory/PickableActor.h"
#include "WeaponBase.generated.h"


class USoundCue;
class UAnimMontage;
class AVincentBloodberry;


UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Firing		UMETA(DisplayName = "Firing"),
	Reloading	UMETA(DisplayName = "Reloading"),
	Equipping	UMETA(DisplayName = "Equiping"),
};


UENUM(BlueprintType)
enum class EWeapons : uint8
{
	Pistol	UMETA(DisplayName = "Pistol"),
	Melee	UMETA(DisplayName = "Melee"),
	Scope	UMETA(DisplayName = "Scope"),
	Mine	UMETA(DisplayName = "Mine"),
};


USTRUCT()
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	/* inifite ammo for reloads */
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	bool bInfiniteAmmo;

	/* infinite ammo in clip, no reload required */
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	bool bInfiniteClip;

	/* max ammo */
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	int32 MaxAmmo;

	/* clip size */
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	int32 AmmoPerClip;

	/* initial clips */
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	int32 InitialClips;

	/* time between two consecutive shots */
	UPROPERTY(EditDefaultsOnly, Category = "WeaponStat")
	float TimeBetweenShots;

	/* reload duration */
	UPROPERTY(EditDefaultsOnly, Category = "WeaponStat")
	float ReloadDuration;

	/* equip/unequip duration */
	UPROPERTY(EditDefaultsOnly, Category = "WeaponStat")
	float EquipDuration;

	/* defaults */
	FWeaponData()
	{
		bInfiniteAmmo = false;
		bInfiniteClip = false;
		MaxAmmo = 100;
		AmmoPerClip = 20;
		InitialClips = 4;
		TimeBetweenShots = 0.2f;
		ReloadDuration = 1.f;
		EquipDuration = 1.f;
	}
};


UCLASS()
class HITCHED_API AWeaponBase : public APickableActor //AActor
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	AWeaponBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*  */
	UPROPERTY(VisibleAnywhere, Category = "Weapon Config")
	FWeaponData WeaponConfig;

	/*  */
	UPROPERTY(VisibleAnywhere, Category = "Weapon Config")
	EWeapons WeaponType;

	/*  */
	UPROPERTY(VisibleAnywhere, Category = "Attaching | Relative Location")
	FVector AttachWeaponRelativeLocation;

    /*  */
	UPROPERTY(VisibleAnywhere, Category = "Attaching | Relative Rotation")
	FRotator AttachWeaponRelativeRotation;

#pragma region SOUNDS

	/* single fire sound (bLoopedFireSound not set) */
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* FireSound = nullptr;

	/* finished burst sound (bLoopedFireSound set) */
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* FireFinishSound = nullptr;

	/* out of ammo sound */
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* OutOfAmmoSound = nullptr;

	/* reload sound */
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* ReloadSound = nullptr;

	/* equip sound */
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* EquipSound = nullptr;

#pragma endregion

#pragma region ANIMATIONS

	/* reload animation */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* ReloadAnim = nullptr;

	/* equip animation */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* EquipAnim = nullptr;

	/* unequip animation */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* UnequipAnim = nullptr;

	/* fire animation */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* FireAnim = nullptr;

#pragma endregion

#pragma region COOLDOWNS

	/** Handle for efficient management of OnEquipFinished timer */
	FTimerHandle OnEquipFinishedCooldown;

	/** Handle for efficient management of ReloadWeapon timer */
	FTimerHandle ReloadWeaponCooldown;

	/** Handle for efficient management of HandleFiring timer */
	FTimerHandle FiringCooldown;

#pragma endregion

private:

	/* current state of weapon */
	UPROPERTY(EditDefaultsOnly, Category = "WeaponState")
	EWeaponState CurrentWeaponState;

public:

	void StartFire();

	void Reload();

	void Equip();

	void Unequip();

	FVector GetAttachLocation() const { return AttachWeaponRelativeLocation; }

	FRotator GetAttachRotation() const { return AttachWeaponRelativeRotation; }

	UFUNCTION(BlueprintCallable)
	EWeapons GetWeaponType() const { return WeaponType; }

protected:

	/* update weapon state */
	void SetWeaponState(EWeaponState NewState);

	/*  */
	void UpdateConfig(FWeaponData NewData);

	/* play weapon sounds */
	void PlayWeaponSound(USoundCue* Sound);

	/* play weapon animations */
	void PlayWeaponAnimation(UAnimMontage* const Animation);

	/* stop playing weapon animations */
	void StopWeaponAnimation(UAnimMontage* const Animation);

private:

	void OnEndFire();

	void OnEndReload();

	void OnEndEquip();

	void OnEndUnequip();

};
