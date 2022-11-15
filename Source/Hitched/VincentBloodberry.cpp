/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "VincentBloodberry.h"
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
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	Camera->SetupAttachment(CameraCollision);
	Camera->SetFieldOfView(100.f);
	Camera->bUsePawnControlRotation = true;

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
	
	// Movement settings
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->AirControl = 0.f;
	GetCharacterMovement()->SetWalkableFloorAngle(60);
	GetCharacterMovement()->MaxAcceleration = 1024.f;				// Inertia movement param
	GetCharacterMovement()->BrakingFrictionFactor = 0.f;			// Inertia movement param
	GetCharacterMovement()->GroundFriction = 4.f;					// Inertia movement param
	GetCharacterMovement()->BrakingDecelerationWalking = 512.f;		// Inertia movement param
	GetCharacterMovement()->PerchRadiusThreshold = 20.f;			// Radius which character can perch the ledge
}

// Called when the game starts or when spawned
void AVincentBloodberry::BeginPlay()
{
	Super::BeginPlay();

	if (HeadBobCurve) {
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

	UpdateMovementCharacteristics(EMovementState::Walk);
}

// Called every frame
void AVincentBloodberry::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleHeadBob(DeltaTime);
	HandleCameraTransforms();
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
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Head bob started")));
			HeadBobTAnim.Play();
		}
	}
	else
	{
		if (HeadBobTAnim.IsPlaying())
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Head bob stopped")));
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
	if (MovementDataMap.Contains(NewMovementState))
	{
		CurrentMovementCharacteristics = MovementDataMap[NewMovementState];

		GetCharacterMovement()->MaxWalkSpeed = CurrentMovementCharacteristics.MoveSpeed;
		GetCapsuleComponent()->SetCapsuleHalfHeight(CurrentMovementCharacteristics.CapsuleHalfHeight);
	}
}

void AVincentBloodberry::MoveForward(float Scale)
{
	if (Scale != 0.0f && CurrentMovementCharacteristics.bCanMove)
	{
		AddMovementInput(GetActorForwardVector(), Scale);
	}
}

void AVincentBloodberry::MoveRight(float Scale)
{
	const float DeltaTime = GetWorld()->DeltaTimeSeconds;

	if (Scale != 0.0f && CurrentMovementCharacteristics.bCanMove)
	{
		AddMovementInput(GetActorRightVector(), Scale);
	}

	CurrentCameraTiltRoll = FMath::FInterpTo(CurrentCameraTiltRoll, CameraTiltAngle * Scale, DeltaTime, CameraTiltSpeed);
}

void AVincentBloodberry::OnLeaning(float Scale)
{
	if (CurrentMovementCharacteristics.bCanLean)
	{
		const float DeltaTime = GetWorld()->DeltaTimeSeconds;

		CurrentCameraLeanY = FMath::FInterpTo(CurrentCameraLeanY, LeanDistance * Scale, DeltaTime, LeanSpeed);
		CurrentCameraLeanRoll = FMath::FInterpTo(CurrentCameraLeanRoll, LeanAngle * Scale, DeltaTime, LeanSpeed);
	}
}

void AVincentBloodberry::StartRunning()
{
	if (CurrentMovementCharacteristics.bCanMove)
	{
		GetCharacterMovement()->MaxWalkSpeed = CurrentMovementCharacteristics.FastMoveSpeed;
	}
}

void AVincentBloodberry::StopRunning()
{
	if (CurrentMovementCharacteristics.bCanMove)
	{
		GetCharacterMovement()->MaxWalkSpeed = CurrentMovementCharacteristics.MoveSpeed;
	}
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

void AVincentBloodberry::InitMovementCharacteristics()
{
	FMovementCharacteristics MovementWalking;
	MovementWalking.MoveSpeed = 111.f;
	MovementWalking.FastMoveSpeed = 208.f;
	MovementWalking.CapsuleHalfHeight = 91.f;
	MovementWalking.bCanMove = true;
	MovementWalking.bCanLean = true;

	FMovementCharacteristics MovementCrouching;
	MovementCrouching.MoveSpeed = 60.f;
	MovementCrouching.FastMoveSpeed = 100.f;
	MovementCrouching.CapsuleHalfHeight = 45.f;
	MovementCrouching.bCanMove = true;
	MovementCrouching.bCanLean = true;

	FMovementCharacteristics MovementVaulting;
	MovementCrouching.MoveSpeed = 0.f;
	MovementCrouching.FastMoveSpeed = 0.f;
	MovementCrouching.CapsuleHalfHeight = 45.f;
	MovementCrouching.bCanMove = false;
	MovementCrouching.bCanLean = false;

	FMovementCharacteristics MovementRopeClimbing;
	MovementRopeClimbing.MoveSpeed = 0.f;
	MovementRopeClimbing.FastMoveSpeed = 0.f;
	MovementRopeClimbing.CapsuleHalfHeight = 91.f;
	MovementRopeClimbing.bCanMove = false;
	MovementRopeClimbing.bCanLean = false;

	MovementDataMap.Add(EMovementState::Walk, MovementWalking);
	MovementDataMap.Add(EMovementState::Crouch, MovementCrouching);
	MovementDataMap.Add(EMovementState::Vault, MovementVaulting);
	MovementDataMap.Add(EMovementState::RopeClimb, MovementRopeClimbing);
}