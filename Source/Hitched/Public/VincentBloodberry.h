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

/* Перечисление всех возможных состояний передвижения персонажа */
UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Walk		UMETA(DisplayName = "Walking"),
	Crouch		UMETA(DisplayName = "Crouching"),
	Vault		UMETA(DisplayName = "Vaulting"),
	RopeClimb	UMETA(DisplayName = "Rope Climbing")
};

/* Параметры передвижения для каждого состояния */
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

	/* Возвращает указатель на camera component персонажа */
	UFUNCTION()
	UCameraComponent* GetFirstPersonCamera() const { return Camera; }

	/* Возвращает указатель на голову персонажа */
	UFUNCTION()
	USphereComponent* GetHead() const { return CameraCollision; }

	/* Для регулировки положения сцен захвата для LightGem */
	UFUNCTION(BlueprintCallable)
	UChildActorComponent* GetChildComp() const { return LightGem; }

protected:

	/* Camera component персонажа */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera = nullptr;

	/* Ыphere component персонажа с коллизией. Защищает камеру от проникновения вовнутрь тектур */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CameraCollision = nullptr;

	/* Actor component персонажа, который содержит индикатор видимости, называемый [Light Gem] */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UChildActorComponent* LightGem = nullptr;

private:

#pragma region FUNCTIONS

	/* Sets default values for this character's properties */
	AVincentBloodberry();

	/* Инициализирует параметры передвижения для каждого состояния */
	void InitMovementCharacteristics();

	/* Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/* Called every frame */
	virtual void Tick(float DeltaTime) override;

	/* Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*
	* В этом методе идет пересчет положения камеры для создания
	* эффекта покачивания камеры во время движения.
	* Вызывается каждый кадр в методе Tick()
	*/
	void HandleHeadBob(float DeltaTime);

	/* Меняет положение головы, учитывая тильт, наклоны и т.д. */
	void HandleCameraTransforms();

	/* Обновляет параметры передвижения персонажа */
	void UpdateMovementCharacteristics(EMovementState NewMovementState);

	/*
	* Переещает персонажа вперед/назад, когда кнопка движения нажата
	* @param Scale получена из Input Component
	*/
	void MoveForward(float Scale);

	/*
	* Переещает персонажа влево/вправо, когда кнопка движения нажата
	* @param Scale получена из Input Component
	*/
	void MoveRight(float Scale);

	/* Персонаж начинает бег, когда нажата кнопка */
	void StartRunning();

	/* Прекращает бег, когда кнопка отпущена */
	void StopRunning();

	/*
	* Начинает/прекращает делать наклоны в стороны, когда определенная
	* кнопка нажата/отпущена
	* @param Scale получена из Input Component
	*/
	void OnLeaning(float Scale);

	/* Таймлайн функция, отвечающая за плавное покачивание камеры. Биндится в HeadbobTAnim */
	UFUNCTION()
	void HeadBobTAnimProgress();

	/* Ивент, срабатывающий в HeadbobTAnim в определенных точках времени */
	UFUNCTION()
	void MakeFootstep();

#pragma endregion

#pragma region VARIABLES

	/* Местонахождение головы персонажа по умолчанию */
	UPROPERTY(VisibleAnywhere, Category = "Components | Camera Collision")
	FVector CameraCollisionDefaultLocation;

	/* Поворот головы персонажа по умолчанию */
	UPROPERTY(VisibleAnywhere, Category = "Components | Camera Collision")
	FRotator CameraCollisionDefaulRotation;

	/* Содержит параметры передвижения для каждого состояния */
	UPROPERTY(VisibleAnywhere, Category = "Movement | Character's Properties")
	TMap<EMovementState, FMovementCharacteristics> MovementDataMap;

	/* Теукщие параметры передвижения (Скорость ходьбы, размер капсулы и т.д.) */
	UPROPERTY(VisibleAnywhere, Category = "Movement | Character's Properties")
	FMovementCharacteristics CurrentMovementCharacteristics;

	/* Значение угла крена Camera tilt в текущий момент. Изменятеся каждый кадр */
	UPROPERTY(VisibleAnywhere, Category = "Movement | Camera Tilt")
	float CurrentCameraTiltRoll = 0.f;

	/* Угол, на который камера может крениться, когда персонаж стрейфится влево/вправо */
	UPROPERTY(VisibleAnywhere, Category = "Movement | Camera Tilt")
	float CameraTiltAngle = 0.7f;

	/* Скорость крена Camera tilt */
	UPROPERTY(VisibleAnywhere, Category = "Movement | Camera Tilt")
	float CameraTiltSpeed = 5.f;

	/* Vector curve для плавного покачивания камеры */
	UPROPERTY(VisibleAnywhere, Category = "Movement | Head Bob")
	UCurveVector* HeadBobCurve = nullptr;

	/* Timeline для покачивания камеры, когда персонаж передвигается */
	UPROPERTY(VisibleAnywhere, Category = "Movement | Head Bob")
	FTimeline HeadBobTAnim;

	/* Звуки шагов персонажа */
	UPROPERTY(VisibleAnywhere, Category = "SFX | Foot Step")
	USoundCue* FootStepSound = nullptr;

	/* Текущее относительное местоположение головы по оси Y. Менятеся каждый кадр */
	UPROPERTY(VisibleAnywhere, Category = "Actions | Leaning")
	float CurrentCameraLeanY = 0.f;

	/* Текущее значение угла крена головы, когда персонаж наклоняется. Меняется каждый кадр */
	UPROPERTY(VisibleAnywhere, Category = "Actions | Leaning")
	float CurrentCameraLeanRoll = 0.f;

	/* Расстояние, на которое персонаж может наклониться */
	UPROPERTY(VisibleAnywhere, Category = "Actions | Leaning")
	float LeanDistance = 100.f;

	/* Угол, на который камера может повернуться, когда персонаж наклоняется */
	UPROPERTY(VisibleAnywhere, Category = "Actions | Leaning")
	float LeanAngle = 5.f;

	/* Скорость наклона */
	UPROPERTY(VisibleAnywhere, Category = "Actions | Leaning")
	float LeanSpeed = 5.f;
	/*ЕБАНЫЙ ГИТ БЛЯТЬ СУКА КАКАОГО ХУЯ?????????*/
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