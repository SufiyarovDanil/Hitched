/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "VincentBloodberry.generated.h"


class UCameraComponent;
class USphereComponent;
class ULightGemComponent;
class UVincentMovementComponent;
class UVincentVaultingComponent;
class UVincentLeaningComponent;
class ULandImpactComponent;
class UVincentActioningComponent;
class UVincentInventoryComponent;
class UAnimMontage;
class UCurveVector;
class UCurveFloat;
class USoundCue;
class AWeaponBase;
class AInventory;


/* Movement state enumerator contains all possible states */
UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Walk		UMETA(DisplayName = "Walking"),
	Crouch		UMETA(DisplayName = "Crouching"),
};


/* Movement settings for each movement state */
USTRUCT()
struct FMovementCharacteristics
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Movement Characteristics")
	float MoveSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Movement Characteristics")
	float FastMoveSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Movement Characteristics | Actions")
	bool bCanMove;

	UPROPERTY(VisibleAnywhere, Category = "Movement Characteristics | Actions")
	bool bCanLean;

	UPROPERTY(VisibleAnywhere, Category = "Movement Characteristics | Actions")
	bool bCanCrouch;
};

/**
 *
 */
UCLASS()
class HITCHED_API AVincentBloodberry : public ACharacter
{
	GENERATED_BODY()

public:

	/* Returns a reference to characters's camera component */
	UFUNCTION()
	UCameraComponent* GetFirstPersonCamera() const { return Camera; }

	/* Returns a reference to character's head */
	UFUNCTION()
	UVincentLeaningComponent* GetHead() const { return LeaningComp; }

	/* For LightGem debugging */
	UFUNCTION(BlueprintCallable)
	ULightGemComponent* GetChildComp() const { return LightGem; }

	/* Returns a reference to character's Inventory component */
	UFUNCTION(BlueprintCallable)
	UVincentInventoryComponent* GetInventoryComponent() const { return InventoryComp; }

	UFUNCTION(BlueprintCallable)
	bool IsLeftHandDrawing() const { return bIsLeftHandDrawing; }

	UFUNCTION(BlueprintCallable)
	bool IsRightHandDrawing() const { return bIsRightHandDrawing; }

	UFUNCTION(BlueprintCallable)
	AWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }

	/* Used to play one of the weapon animations in right hand */
	virtual float PlayAnimMontage(UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None) override;

	virtual void StopAnimMontage(UAnimMontage* AnimMontage) override;

protected:

	/* Character's camera component */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera = nullptr;

	/* Character's sphere component with collision. Protecting camera from wall clipping... maybe... */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CameraCollision = nullptr;

	/* Character's left hand skeletal mesh */
	UPROPERTY(VisibleAnywhere, Category = "Hands")
	USkeletalMeshComponent* LeftHand = nullptr;

	/* Character's right hand skeletal mesh */
	UPROPERTY(VisibleAnywhere, Category = "Hands")
	USkeletalMeshComponent* RightHand = nullptr;

	/* Character's scene component that contains light detector called [Light Gem] */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	ULightGemComponent* LightGem = nullptr;

	/* Characters's modified movement component */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UVincentMovementComponent* MovementComp = nullptr;

	/* Character's vaulting component */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UVincentVaultingComponent* VaultingComp = nullptr;

	/* Character's leaning component */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UVincentLeaningComponent* LeaningComp = nullptr;

	/* Character's actioning component */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UVincentActioningComponent* ActioningComp = nullptr;

	/* Character's inventory component */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UVincentInventoryComponent* InventoryComp = nullptr;

	/* Character's equiped weapon */
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeaponBase* CurrentWeapon = nullptr;

private:

#pragma region FUNCTIONS

	/* Sets default values for this character's properties */
	AVincentBloodberry(const FObjectInitializer& ObjectInitializer);

	/* Initializes movement characteristics for each movement state */
	void InitMovementCharacteristics();

	/* Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/* Called every frame */
	virtual void Tick(float DeltaTime) override;

	/* Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Handles head bob process. Calling in every tick */
	void HandleHeadBob(float DeltaTime);

	/* Changes camera transforms every frame, considering tilt, lean etc */
	void HandleCameraTransforms(float DeltaTime);

	/* Updates movement characteristics when */
	void UpdateMovementCharacteristics(EMovementState NewMovementState);

#pragma region INPUTS

	/* Move the character forward/backward when move button is pressed
	* @param Scale The value passed in by the Input Component
	*/
	void MoveForward(float Scale);

	/* Move the character left/right when move button is released
	* @param Scale The value passed in by the Input Component
	*/
	void MoveRight(float Scale);

	/* Starting to sprint when sprint button is pressed */
	void StartRunning();

	/* Stopping to sprint when sprint button is released */
	void StopRunning();

	/* Toggling MovementMod between Crouch and Walk */
	void ToggleCrouch();

	/* Interacting with interactable actors */
	void DoAction();

	/* Calling when character starts Jumping */
	virtual void Jump() override;

	/* Calling when character starts  */
	void LeanLeft();

	/* */
	void UnleanLeft();

	/* */
	void LeanRight();

	/* */
	void UnleanRight();

	/* Drawing or Hiding left hand */
	void ToggleShowWatch();

	/* Drawing or Hiding right hand */
	void ToggleDrawRightHand();

	/* */
	void StartFiring();

	/* Reloads weapon */
	void ReloadWeapon();

	/* */
	void ToggleInventory();

	/* */
	void TakeSelectedItemFromInventory();

#pragma endregion

	/* */
	void AttachWeaponToHand();

	/* */
	void DetachWeaponFromHand();

	/* overrided function of ACharacter class */
	virtual bool CanJumpInternal_Implementation() const override;

	/* Overrided function of ACharacter class */
	virtual void Landed(const FHitResult& Hit) override;

	/* Calling when character starts crouch */
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	/* Calling when character starts uncrouch */
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	/* Ticks the head bob timeline */
	UFUNCTION()
	void HeadBobTAnimProgress();

	/* Event for head bob timeline */
	UFUNCTION()
	void MakeFootstep();

#pragma endregion

	/* Default Camera collision's location */
	UPROPERTY(VisibleAnywhere, Category = "Components | Camera Collision")
	FVector CameraCollisionWalkLocation;

	/* Default Camera collision's location */
	UPROPERTY(VisibleAnywhere, Category = "Components | Camera Collision")
	FRotator CameraCollisionDefaulRotation;

#pragma region MOVEMENT_STATE

	/* Contains all character characteristics for each movement state */
	UPROPERTY(VisibleAnywhere, Category = "Movement | Character's Properties")
	TMap<EMovementState, FMovementCharacteristics> MovementDataMap;

	UPROPERTY(VisibleAnywhere, Category = "Movement | Movement State")
	EMovementState CurrentMovementState;

#pragma endregion

#pragma region HEAD_BOB

	/* Vector curve for smooth head bobbing */
	UPROPERTY(VisibleAnywhere, Category = "Movement | Head Bob")
	FTimeline HeadBobTAnim;

	/* Timeline for head bobbing when character is moving. Generated from the head bob curve */
	UPROPERTY(VisibleAnywhere, Category = "Movement | Head Bob")
	UCurveVector* HeadBobCurve = nullptr;

	/* Character's foot step sound */
	UPROPERTY(VisibleAnywhere, Category = "SFX | Foot Step")
	USoundCue* FootStepSound = nullptr;

#pragma endregion

#pragma region CROUCH

	/* Character's Capsule half height when is in crouch movement state */
	UPROPERTY(VisibleAnywhere, Category = "Crouching")
	float CrouchCapsuleHalfHeight;

	/* Character's Capsule half height when is walking */
	UPROPERTY(VisibleAnywhere, Category = "Crouching")
	float CapsuleHalfHeight;

	/* Target Location for Head interp */
	UPROPERTY(VisibleAnywhere, Category = "Crouching")
	FVector CameraCollisionCrouchLocation;

	/* */
	UPROPERTY(VisibleAnywhere, Category = "Crouching")
	float CrouchSpeed;

#pragma endregion

#pragma region HANDS

	/* */
	UPROPERTY(VisibleAnywhere, Category = "Hands")
	bool bIsLeftHandDrawing;

	/* */
	UPROPERTY(VisibleAnywhere, Category = "Hands")
	bool bIsRightHandDrawing;

#pragma endregion

#pragma region LANDING

	/* Animation Timeline starts when character landed on lower surface */
	UPROPERTY(VisibleAnywhere, Category = "Landing | AnimTimeline")
	ULandImpactComponent* LandImpactComp = nullptr;

	/* That Value rewrites every time when character start jumping */
	UPROPERTY(VisibleAnywhere, Category = "Landing | Jump Start Point")
	FVector JumpStartPoint;

	/* That value rewrites every time when character landing */
	UPROPERTY(VisibleAnywhere, Category = "Landing | Jump End Point")
	FVector JumpEndPoint;

#pragma endregion

#pragma region MOVEMENT_VALUES

	UPROPERTY(VisibleAnywhere, Category = "Movement Value")
	bool bIsRunning = false;

	UPROPERTY(VisibleAnywhere, Category = "Movement Value")
	bool bCanMove = false;

	UPROPERTY(VisibleAnywhere, Category = "Movement Value")
	bool bCanLean = false;

	UPROPERTY(VisibleAnywhere, Category = "Movement Value")
	bool bCanCrouch = false;

#pragma endregion

};
