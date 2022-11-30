/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "VincentBloodberry.h"
#include "LightGem.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Curves/CurveVector.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AVincentBloodberry::AVincentBloodberry()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitMovementCharacteristics();
	CurrentMovementState = EMovementState::Walk;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(45.5f, 91.f); // Radius and half heigh, so we need to divide Vincent's height by 2 (182 / 2)

	// Set head parameters
	CameraCollisionDefaultLocation = FVector(0.f, 0.f, 60.f);
	CameraCollisionDefaulRotation = FRotator(0.f, 0.f, 0.f);

	CameraCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Head"));
	CameraCollision->SetupAttachment(GetCapsuleComponent());
	CameraCollision->SetRelativeLocation(CameraCollisionDefaultLocation);
	CameraCollision->SetSphereRadius(30.f);

	// Set camera parameters
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	Camera->SetupAttachment(CameraCollision);
	Camera->SetFieldOfView(100.f);
	Camera->bUsePawnControlRotation = true;

	// Set light detector
	LightGem = CreateDefaultSubobject<UChildActorComponent>(TEXT("Light Gem"));
	LightGem->SetupAttachment(GetCapsuleComponent());
	LightGem->SetChildActorClass(ALightGem::StaticClass());
	LightGem->SetRelativeLocation(CameraCollisionDefaultLocation);
	LightGem->SetUsingAbsoluteRotation(true); // When character yaw rotating, light gem output value changes. That's why we need to lock light gem rotation

	// Head bob curve init
	static ConstructorHelpers::FObjectFinder<UCurveVector> HeadBobCurveAsset(
		TEXT("CurveVector'/Game/Vincent/TimelineAnims/HeadBobCurve.HeadBobCurve'"));

	if (HeadBobCurveAsset.Succeeded())
	{
		HeadBobCurve = HeadBobCurveAsset.Object;
	}

	// Sound base init
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCueAsset(
		TEXT("SoundCue'/Game/Vincent/SFX/FootstepCue.FootstepCue'"));

	if (SoundCueAsset.Succeeded())
	{
		FootStepSound = SoundCueAsset.Object;
	}

	// Crouch curve
	static ConstructorHelpers::FObjectFinder<UCurveFloat> CrouchCurveAsset(
		TEXT("CurveFloat'/Game/Vincent/TimelineAnims/CrouchCurve.CrouchCurve'"));

	if (CrouchCurveAsset.Succeeded())
	{
		CrouchCurve = CrouchCurveAsset.Object;
	}

	// Movement settings
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->AirControl = 0.f;
	GetCharacterMovement()->SetWalkableFloorAngle(60);

	// Inertia params
	GetCharacterMovement()->MaxAcceleration = 1024.f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.f;
	GetCharacterMovement()->GroundFriction = 4.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 512.f;

	GetCharacterMovement()->PerchRadiusThreshold = 20.f;
}

// Called when the game starts or when spawned
void AVincentBloodberry::BeginPlay()
{
	Super::BeginPlay();

	if (HeadBobCurve)
	{
		FOnTimelineVector TimelineCallback;
		FOnTimelineEvent FootstepEvent;

		TimelineCallback.BindUFunction(this, FName(TEXT("HeadBobTAnimProgress")));
		FootstepEvent.BindUFunction(this, FName(TEXT("MakeFootstep")));

		HeadBobTAnim.AddInterpVector(HeadBobCurve, TimelineCallback);
		HeadBobTAnim.AddEvent(50.f, FootstepEvent);
		HeadBobTAnim.AddEvent(150.f, FootstepEvent);
		HeadBobTAnim.SetLooping(true);
		HeadBobTAnim.SetTimelineLength(200.f);
	}

	if (CrouchCurve)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Crouching222")));
		FOnTimelineFloat Callback;
		Callback.BindUFunction(this, FName(TEXT("CrouchTAnimProgress")));

		CrouchTAnim.AddInterpFloat(CrouchCurve, Callback);
		CrouchTAnim.SetLooping(false);
		CrouchTAnim.SetTimelineLength(0.4f);
	}

	if (MovementDataMap.Contains(EMovementState::Walk))
	{
		CurrentMovementState = EMovementState::Walk;
		UpdateMovementCharacteristics(CurrentMovementState);
	}
}

// Called every frame
void AVincentBloodberry::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleHeadBob(DeltaTime);
	HandleCameraTransforms();
	CrouchTAnim.TickTimeline(DeltaTime);
}

// Called to bind functionality to input
void AVincentBloodberry::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Mouse things
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up", this, &APawn::AddControllerPitchInput);

	// Action input
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindKey(EKeys::C, IE_Pressed, this, &AVincentBloodberry::ToggleCrouch);

	PlayerInputComponent->BindAxis("Lean", this, &AVincentBloodberry::OnLeaning);

	// Movement input
	PlayerInputComponent->BindAxis("Move Forward", this, &AVincentBloodberry::MoveForward);
	PlayerInputComponent->BindAxis("Move Right", this, &AVincentBloodberry::MoveRight);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AVincentBloodberry::StartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AVincentBloodberry::StopRunning);
}

void AVincentBloodberry::HandleHeadBob(float DeltaTime) // check character movement method InMovingOnGround()
{
	const float Velocity = GetVelocity().Size2D();
	const bool bCanHeadBob = GetCharacterMovement()->IsMovingOnGround() && Velocity > 0.f;

	if (bCanHeadBob)
	{
		if (!HeadBobTAnim.IsPlaying())
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Head bob started")));
			HeadBobTAnim.Play();
		}
	}
	else
	{
		if (HeadBobTAnim.IsPlaying())
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Head bob stopped")));
			HeadBobTAnim.Stop();
		}
	}

	HeadBobTAnim.TickTimeline(Velocity * DeltaTime);
}

void AVincentBloodberry::HandleCameraTransforms()
{
	const FRotator CurrentControlRotation = GetController()->GetControlRotation();
	const float CalculatedCameraRoll = CurrentCameraTiltRoll + CurrentCameraLeanRoll;
	const FRotator CalculatedCameraRotation(CurrentControlRotation.Pitch, CurrentControlRotation.Yaw, CalculatedCameraRoll);

	const FVector CurrentCameraCollLocation = GetHead()->GetRelativeLocation();
	const FVector CalculatedCameraCollLocation(CurrentCameraCollLocation.X, CurrentCameraLeanY, CurrentCameraCollLocation.Z);

	GetHead()->SetRelativeLocation(CalculatedCameraCollLocation);
	GetController()->SetControlRotation(CalculatedCameraRotation);
}

void AVincentBloodberry::UpdateMovementCharacteristics(EMovementState NewMovementState)
{
	if (!MovementDataMap.Contains(NewMovementState))
	{
		return;
	}

	CurrentMovementState = NewMovementState;

	GetCharacterMovement()->MaxWalkSpeed =
		(bIsRunning) ? MovementDataMap[CurrentMovementState].FastMoveSpeed : MovementDataMap[CurrentMovementState].MoveSpeed;

	bCanMove = MovementDataMap[CurrentMovementState].bCanMove;
	bCanLean = MovementDataMap[CurrentMovementState].bCanLean;
	bCanCrouch = MovementDataMap[CurrentMovementState].bCanCrouch;

	/*switch (NewMovementState)
	{
	case EMovementState::Walk:
	{
		break;
	}
	case EMovementState::Crouch:
	{
		break;
	}
	case EMovementState::Vault:
	{
		break;
	}
	case EMovementState::RopeClimb:
	{
		break;
	}
	}*/
}

void AVincentBloodberry::MoveForward(float Scale)
{
	if (Scale != 0.0f && bCanMove)
	{
		AddMovementInput(GetActorForwardVector(), Scale);
	}
}

void AVincentBloodberry::MoveRight(float Scale)
{
	const float DeltaTime = GetWorld()->DeltaTimeSeconds;

	if (Scale != 0.0f && bCanMove)
	{
		AddMovementInput(GetActorRightVector(), Scale);
	}

	CurrentCameraTiltRoll = FMath::FInterpTo(CurrentCameraTiltRoll, CameraTiltAngle * Scale, DeltaTime, CameraTiltSpeed);
}

void AVincentBloodberry::OnLeaning(float Scale)
{

	if (!bCanLean)
	{
		return;
	}

	const float DeltaTime = GetWorld()->DeltaTimeSeconds;

	CurrentCameraLeanY = FMath::FInterpTo(CurrentCameraLeanY, LeanDistance * Scale, DeltaTime, LeanSpeed);
	CurrentCameraLeanRoll = FMath::FInterpTo(CurrentCameraLeanRoll, LeanAngle * Scale, DeltaTime, LeanSpeed);
}

void AVincentBloodberry::StartRunning()
{
	if (!bCanMove)
	{
		return;
	}

	bIsRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = MovementDataMap[CurrentMovementState].FastMoveSpeed;
}

void AVincentBloodberry::StopRunning()
{
	if (!bCanMove)
	{
		return;
	}

	bIsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = MovementDataMap[CurrentMovementState].MoveSpeed;
}

void AVincentBloodberry::ToggleCrouch()
{
	if (!bCanCrouch)
	{
		return;
	}

	if (!bIsCrouching)
	{
		UpdateMovementCharacteristics(EMovementState::Crouch);
		CrouchTAnim.Play();
	}
	else
	{
		UpdateMovementCharacteristics(EMovementState::Walk);
		CrouchTAnim.Reverse();
	}

	bIsCrouching = !bIsCrouching;
}

void AVincentBloodberry::HeadBobTAnimProgress()
{
	const float TimelineValue = HeadBobTAnim.GetPlaybackPosition();
	const FVector TimelineLoc = HeadBobCurve->GetVectorValue(TimelineValue);

	Camera->SetRelativeLocation(TimelineLoc);
}

void AVincentBloodberry::MakeFootstep()
{
	if (FootStepSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FootStepSound, GetActorLocation());
	}
}

void AVincentBloodberry::CrouchTAnimProgress()
{
	const float TimelineValue = CrouchTAnim.GetPlaybackPosition();
	const float TimelineFloat = CrouchCurve->GetFloatValue(TimelineValue);
	const float NewHalfHeight = FMath::Lerp(91.f, 50.5f, TimelineFloat);

	GetCapsuleComponent()->SetCapsuleHalfHeight(NewHalfHeight);
}

void AVincentBloodberry::InitMovementCharacteristics()
{
	FMovementCharacteristics MovementWalking{};
	MovementWalking.MoveSpeed = 111.f;
	MovementWalking.FastMoveSpeed = 208.f;
	MovementWalking.bCanMove = true;
	MovementWalking.bCanLean = true;
	MovementWalking.bCanCrouch = true;

	FMovementCharacteristics MovementCrouching{};
	MovementCrouching.MoveSpeed = 60.f;
	MovementCrouching.FastMoveSpeed = 120.f;
	MovementCrouching.bCanMove = true;
	MovementCrouching.bCanLean = true;
	MovementCrouching.bCanCrouch = true;

	FMovementCharacteristics MovementVaulting{};
	MovementVaulting.MoveSpeed = 0.f;
	MovementVaulting.FastMoveSpeed = 0.f;
	MovementVaulting.bCanMove = false;
	MovementVaulting.bCanLean = false;
	MovementVaulting.bCanCrouch = false;

	FMovementCharacteristics MovementRopeClimbing{};
	MovementRopeClimbing.MoveSpeed = 0.f;
	MovementRopeClimbing.FastMoveSpeed = 0.f;
	MovementRopeClimbing.bCanMove = false;
	MovementRopeClimbing.bCanLean = false;
	MovementRopeClimbing.bCanCrouch = false;

	MovementDataMap.Add(EMovementState::Walk, MovementWalking);
	MovementDataMap.Add(EMovementState::Crouch, MovementCrouching);
	MovementDataMap.Add(EMovementState::Vault, MovementVaulting);
	MovementDataMap.Add(EMovementState::RopeClimb, MovementRopeClimbing);
}