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

/* ������������ ���� ��������� ��������� ������������ ��������� */
UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Walk		UMETA(DisplayName = "Walking"),
	Crouch		UMETA(DisplayName = "Crouching"),
	Vault		UMETA(DisplayName = "Vaulting"),
	RopeClimb	UMETA(DisplayName = "Rope Climbing")
};

/* ��������� ������������ ��� ������� ��������� */
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
};

/**
 *
 */
UCLASS()
class HITCHED_API AVincentBloodberry : public ACharacter
{
	GENERATED_BODY()

public:

	/* ���������� ��������� �� camera component ��������� */
	UFUNCTION()
	UCameraComponent* GetFirstPersonCamera() const { return Camera; }

	/* ���������� ��������� �� ������ ��������� */
	UFUNCTION()
	USphereComponent* GetHead() const { return CameraCollision; }

	/* ��� ����������� ��������� ���� ������� ��� LightGem */
	UFUNCTION(BlueprintCallable)
	UChildActorComponent* GetChildComp() const { return LightGem; }

protected:

	/* Camera component ��������� */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera = nullptr;

	/* �phere component ��������� � ���������. �������� ������ �� ������������� �������� ������ */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CameraCollision = nullptr;

	/* Actor component ���������, ������� �������� ��������� ���������, ���������� [Light Gem] */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UChildActorComponent* LightGem = nullptr;

private:

#pragma region FUNCTIONS

	/* Sets default values for this character's properties */
	AVincentBloodberry();

	/* �������������� ��������� ������������ ��� ������� ��������� */
	void InitMovementCharacteristics();

	/* Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/* Called every frame */
	virtual void Tick(float DeltaTime) override;

	/* Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*
	* � ���� ������ ���� �������� ��������� ������ ��� ��������
	* ������� ����������� ������ �� ����� ��������.
	* ���������� ������ ���� � ������ Tick()
	*/
	void HandleHeadBob(float DeltaTime);

	/* ������ ��������� ������, �������� �����, ������� � �.�. */
	void HandleCameraTransforms();

	/* ��������� ��������� ������������ ��������� */
	void UpdateMovementCharacteristics(EMovementState NewMovementState);

	/*
	* ��������� ��������� ������/�����, ����� ������ �������� ������
	* @param Scale �������� �� Input Component
	*/
	void MoveForward(float Scale);

	/*
	* ��������� ��������� �����/������, ����� ������ �������� ������
	* @param Scale �������� �� Input Component
	*/
	void MoveRight(float Scale);

	/* �������� �������� ���, ����� ������ ������ */
	void StartRunning();

	/* ���������� ���, ����� ������ �������� */
	void StopRunning();

	/*
	* ��������/���������� ������ ������� � �������, ����� ������������
	* ������ ������/��������
	* @param Scale �������� �� Input Component
	*/
	void OnLeaning(float Scale);

	/* �������� �������, ���������� �� ������� ����������� ������. �������� � HeadbobTAnim */
	UFUNCTION()
	void HeadBobTAnimProgress();

	/* �����, ������������� � HeadbobTAnim � ������������ ������ ������� */
	UFUNCTION()
	void MakeFootstep();

#pragma endregion

#pragma region VARIABLES

	/* ��������������� ������ ��������� �� ��������� */
	UPROPERTY(VisibleAnywhere, Category = "Components | Camera Collision")
	FVector CameraCollisionDefaultLocation;

	/* ������� ������ ��������� �� ��������� */
	UPROPERTY(VisibleAnywhere, Category = "Components | Camera Collision")
	FRotator CameraCollisionDefaulRotation;

	/* �������� ��������� ������������ ��� ������� ��������� */
	UPROPERTY(VisibleAnywhere, Category = "Movement | Character's Properties")
	TMap<EMovementState, FMovementCharacteristics> MovementDataMap;

	/* ������� ��������� ������������ (�������� ������, ������ ������� � �.�.) */
	UPROPERTY(VisibleAnywhere, Category = "Movement | Character's Properties")
	FMovementCharacteristics CurrentMovementCharacteristics;

	/* �������� ���� ����� Camera tilt � ������� ������. ���������� ������ ���� */
	UPROPERTY(VisibleAnywhere, Category = "Movement | Camera Tilt")
	float CurrentCameraTiltRoll = 0.f;

	/* ����, �� ������� ������ ����� ���������, ����� �������� ���������� �����/������ */
	UPROPERTY(VisibleAnywhere, Category = "Movement | Camera Tilt")
	float CameraTiltAngle = 0.7f;

	/* �������� ����� Camera tilt */
	UPROPERTY(VisibleAnywhere, Category = "Movement | Camera Tilt")
	float CameraTiltSpeed = 5.f;

	/* Vector curve ��� �������� ����������� ������ */
	UPROPERTY(VisibleAnywhere, Category = "Movement | Head Bob")
	UCurveVector* HeadBobCurve = nullptr;

	/* Timeline ��� ����������� ������, ����� �������� ������������� */
	UPROPERTY(VisibleAnywhere, Category = "Movement | Head Bob")
	FTimeline HeadBobTAnim;

	/* ����� ����� ��������� */
	UPROPERTY(VisibleAnywhere, Category = "SFX | Foot Step")
	USoundCue* FootStepSound = nullptr;

	/* ������� ������������� �������������� ������ �� ��� Y. �������� ������ ���� */
	UPROPERTY(VisibleAnywhere, Category = "Actions | Leaning")
	float CurrentCameraLeanY = 0.f;

	/* ������� �������� ���� ����� ������, ����� �������� �����������. �������� ������ ���� */
	UPROPERTY(VisibleAnywhere, Category = "Actions | Leaning")
	float CurrentCameraLeanRoll = 0.f;

	/* ����������, �� ������� �������� ����� ����������� */
	UPROPERTY(VisibleAnywhere, Category = "Actions | Leaning")
	float LeanDistance = 100.f;

	/* ����, �� ������� ������ ����� �����������, ����� �������� ����������� */
	UPROPERTY(VisibleAnywhere, Category = "Actions | Leaning")
	float LeanAngle = 5.f;

	/* �������� ������� */
	UPROPERTY(VisibleAnywhere, Category = "Actions | Leaning")
	float LeanSpeed = 5.f;
	/*������ ��� ����� ���� ������� ���?????????*/
#pragma endregion

#pragma region MOVEMENT_VALUES
	// not implemented
	UPROPERTY()
	bool bIsCrouching = false;

	UPROPERTY()
	bool bIsRunning = false;

	UPROPERTY()
	bool bCanMove = false;

	UPROPERTY()
	bool bCanLean = false;

	UPROPERTY()
	bool bCanCrouch = false;

#pragma endregion

};