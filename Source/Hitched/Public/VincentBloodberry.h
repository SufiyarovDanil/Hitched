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
class UMatineeCameraShake;
class USphereComponent;
class ULightGemComponent;
class UVincentMovementComponent;
class UVincentVaultingComponent;
class UCurveVector;
class UCurveFloat;
class USoundCue;

/* Movement state enumerator contains all possible states */
UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Walk		UMETA(DisplayName = "Walking"),
	Crouch		UMETA(DisplayName = "Crouching"),
	Vault		UMETA(DisplayName = "Vaulting"),
	RopeClimb	UMETA(DisplayName = "Rope Climbing")
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
	USphereComponent* GetHead() const { return CameraCollision; }

	/* For LightGem debugging */
	UFUNCTION(BlueprintCallable)
	ULightGemComponent* GetChildComp() const { return LightGem; }

protected:

	/* Character's camera component */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera = nullptr;

	/* Character's sphere component with collision. Protecting camera from wall clipping... maybe... */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CameraCollision = nullptr;

	/* Character's scene component that contains light detector called [Light Gem] */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	ULightGemComponent* LightGem = nullptr;

	/* Characters's modified movement component */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UVincentMovementComponent* MovementComp = nullptr;

	/* Character's vaulting component */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UVincentVaultingComponent* VaultingComp = nullptr;

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

	/* overrided function of ACharacter class */
	virtual bool CanJumpInternal_Implementation() const override;

	/* Overrided function of ACharacter class */
	virtual void Landed(const FHitResult& Hit) override;

	/* Calling when character start crouch */
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	/* Calling when character start uncrouch */
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	/* Calling when character start Jumping */
	virtual void Jump() override;

	/* Starting/stopping to lean when lean buttons are pressed/released
	*  @param Scale the value passed in by the Input Component
	*/
	void OnLeaning(float Scale);

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

#pragma region TILT

	/* The angle at which the camera tilt roll is currently located. Changes every frame */
	UPROPERTY(VisibleAnywhere, Category = "Movement | Camera Tilt")
	float CurrentCameraTiltRoll = 0.f;

	/* The angle which camera have roll when character is stafing to left/right */
	UPROPERTY(VisibleAnywhere, Category = "Movement | Camera Tilt")
	float CameraTiltAngle = 0.5f;

	/* The speed of camera tilting */
	UPROPERTY(VisibleAnywhere, Category = "Movement | Camera Tilt")
	float CameraTiltSpeed = 5.f;

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
	UPROPERTY()
	float CrouchCapsuleHalfHeight;

	/* Character's Capsule half height when is walking */
	UPROPERTY()
	float CapsuleHalfHeight;

	/* Target Location for Head interp */
	UPROPERTY()
	FVector CameraCollisionCrouchLocation;

	/* */
	UPROPERTY()
	float CrouchSpeed;

#pragma endregion

#pragma region LEAN

	/* Current head collision's Y location. Changes every frame */
	UPROPERTY(VisibleAnywhere, Category = "Actions | Leaning")
	float CurrentCameraLeanY = 0.f;

	/* The angle at which the camera lean roll is currently located. Changes every frame */
	UPROPERTY(VisibleAnywhere, Category = "Actions | Leaning")
	float CurrentCameraLeanRoll = 0.f;

	/* The distance how far head can move from head origin position */
	UPROPERTY(VisibleAnywhere, Category = "Actions | Leaning")
	float LeanDistance = 100.f;

	/* Angle to which the camera should roll while leaning */
	UPROPERTY(VisibleAnywhere, Category = "Actions | Leaning")
	float LeanAngle = 5.f;

	/* the speed of leaning */
	UPROPERTY(VisibleAnywhere, Category = "Actions | Leaning")
	float LeanSpeed = 5.f;

#pragma endregion

#pragma region LANDING

	/* Camera Shake that gives effective impact when character landing on any floor */
	UPROPERTY(VisibleAnywhere, Category = "Landing")
	TSubclassOf<UMatineeCameraShake> LandingCamShake = nullptr;

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
