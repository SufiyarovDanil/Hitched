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
	Crouch		UMETA(DisplayName = "Crouching"),
	Vault		UMETA(DisplayName = "Vaulting"),
	RopeClimb	UMETA(DisplayName = "Rope Climbing")
};

/* Movement settings for each movement state */
USTRUCT()
struct FMovementCharacteristics
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Movement Characteristics")
	float MoveSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Movement Characteristics")
	float FastMoveSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Movement Characteristics")
	float CapsuleHalfHeight;

	UPROPERTY(VisibleAnywhere, Category = "Movement Characteristics | Actions")
	bool bCanMove;

	UPROPERTY(VisibleAnywhere, Category = "Movement Characteristics | Actions")
	bool bCanLean;

	//UPROPERTY(VisibleAnywhere, Category = "Movement Characteristics | Actions")
	//bool bCanAttack;
};

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

protected:

	/* Character's camera component */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera = nullptr;

	/* Character's sphere component with collision. Protecting camera from wall clipping... maybe... */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CameraCollision = nullptr;

private:

#pragma region FUNCTIONS

	/* Sets default values for this character's properties */
	AVincentBloodberry();

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

	/* Default Camera collision's location */
	UPROPERTY(VisibleAnywhere, Category = "Components | Camera Collision")
	FVector CameraCollisionDefaultLocation;

	/* Default Camera collision's location */
	UPROPERTY(VisibleAnywhere, Category = "Components | Camera Collision")
	FRotator CameraCollisionDefaulRotation;

	UPROPERTY(VisibleAnywhere, Category = "Movement | Character's properties")
	TMap<EMovementState, FMovementCharacteristics> MovementDataMap;

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