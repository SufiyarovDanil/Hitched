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
class UCurveVector;
class UCurveFloat;
class USoundCue;

/* Movement state enumerator contains all possible states */
UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Walk		UMETA(DisplayName = "Walking"),
	Sprint		UMETA(DisplayName = "Srinting"),
	Crouch		UMETA(DisplayName = "Crouching"),
	FastCrouch	UMETA(DisplayName = "Fast crouching"),
	Vault		UMETA(DisplayName = "Vaulting"),
	RopeClimb	UMETA(DisplayName = "Rope Climbing")
};

/* Movement settings for each movement state */
USTRUCT()
struct FMovementCharacteristics
{
	GENERATED_BODY()

	float MoveSpeed;
	float FootstepVolume;
	float CapsuleHalfHeight;

	bool bCanMove;
	bool bCanLean;
	bool bCanAttack;
	bool bCanStanUp;
	bool bCanVault;
	bool bCanJump;
	bool bCan;
};

UCLASS()
class HITCHED_API AVincentBloodberry : public ACharacter
{
	GENERATED_BODY()

public:

	/* Returns a reference to characters's camera component */
	UFUNCTION()
	UCameraComponent* GetFirstPersonCamera() const { return FirstPersonCamera; }

	/* Returns a reference to character's head */
	UFUNCTION()
	USphereComponent* GetHead() const { return Head; }

protected:

	/* Character's camera component */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* FirstPersonCamera = nullptr;

	/* Character's sphere component with collision. Protecting camera from wall clipping... maybe... */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* Head = nullptr;

private:

#pragma region FUNCTIONS

	/* Sets default values for this character's properties */
	AVincentBloodberry();

	/* Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/* Called every frame */
	virtual void Tick(float DeltaTime) override;

	/* Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Handles head bob process. Calling in every tick */
	void HandleHeadBob(float DeltaTime);

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

	/* Starting/stopping to lean when lean buttons are pressed/released */
	void OnLeaning(float Scale);

	/* Ticks the head bob timeline */
	UFUNCTION()
	void HeadBobTAnimProgress();

	/* Event for head bob timeline */
	UFUNCTION()
	void MakeFootstep();

#pragma endregion

#pragma region VARIABLES

	/* Default Head's position */
	UPROPERTY(VisibleAnywhere, Category = "Components | Head")
	FVector HeadOriginLocation;

	UPROPERTY(VisibleAnywhere, Category = "Components | Head")
	FRotator HeadOriginRotation;

	UPROPERTY(VisibleAnywhere, Category = "Movement | Character's properties")
	FMovementCharacteristics CurrentMovementCharacteristics;

	/* Vector curve for smooth head bobbing */
	UPROPERTY(VisibleAnywhere, Category = "Movement | Head bob")
	UCurveVector* HeadBobCurve = nullptr;

	/* Timeline for head bobbing when character is moving. Generated from the head bob curve */
	UPROPERTY(VisibleAnywhere, Category = "Movement | Head bob")
	FTimeline HeadBobTAnim;

	/* Character's foot step sound */
	UPROPERTY(VisibleAnywhere, Category = "SFX | Foot step")
	USoundCue* FootStepSound = nullptr;

	/* The distance how far head can move from head origin position */
	UPROPERTY(VisibleAnywhere, Category = "Actions | Leaning")
	float LeanDistance = 100.f;

	/* Tilt to which the camera should roll while leaning */
	UPROPERTY(VisibleAnywhere, Category = "Actions | Leaning")
	float LeanRotation = 5.f;

	/* the speed of leaning */
	UPROPERTY(VisibleAnywhere, Category = "Actions | Leaning")
	float LeanSpeed = 5.f;

#pragma endregion
};