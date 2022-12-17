/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "VincentBloodberry.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "LightGemComponent.h"
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

	CapsuleHalfHeight = 91.f;
	CrouchCapsuleHalfHeight = 50.f;
	CrouchSpeed = 10.f;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(45.5f, CapsuleHalfHeight); // Radius and half heigh, so we need to divide Vincent's height by 2 (182 / 2)

	// Set head parameters
	const float CameraCollisionRadius = 30.f;
	CameraCollisionWalkLocation = FVector(0.f, 0.f, CapsuleHalfHeight - CameraCollisionRadius);
	CameraCollisionCrouchLocation = FVector(0.f, 0.f, CrouchCapsuleHalfHeight - CameraCollisionRadius);
	CameraCollisionDefaulRotation = FRotator(0.f, 0.f, 0.f);

	CameraCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Head"));
	CameraCollision->SetupAttachment(GetCapsuleComponent());
	CameraCollision->SetRelativeLocation(CameraCollisionWalkLocation);
	CameraCollision->SetSphereRadius(CameraCollisionRadius);

	// Set camera parameters
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	Camera->SetupAttachment(CameraCollision);
	Camera->SetFieldOfView(100.f);
	Camera->bUsePawnControlRotation = true;

	LightGem = CreateDefaultSubobject<ULightGemComponent>(TEXT("Light Gem"));
	LightGem->SetupAttachment(GetCapsuleComponent());
	LightGem->SetRelativeLocation(CameraCollisionWalkLocation);
	// When character yaw rotating, light gem output value changes. That's why we need to lock light gem rotation
	LightGem->SetUsingAbsoluteRotation(true);

	// Head bob curve init
	static ConstructorHelpers::FObjectFinder<UCurveVector> HeadBobCurveAsset(
		TEXT("CurveVector'/Game/Vincent/TimelineAnims/HeadBobCurve.HeadBobCurve'"));

	if (HeadBobCurveAsset.Succeeded())
	{
		HeadBobCurve = HeadBobCurveAsset.Object;
	}

	// Sound base init
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCueAsset(
		TEXT("SoundCue'/Game/Vincent/SFX/Footstep/FootstepQue.FootstepQue'"));

	if (SoundCueAsset.Succeeded())
	{
		FootStepSound = SoundCueAsset.Object;
	}

	// Movement settings
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->AirControl = 0.f;
	GetCharacterMovement()->CrouchedHalfHeight = CrouchCapsuleHalfHeight;
	GetCharacterMovement()->SetWalkableFloorAngle(60);

	// Movement Nav properties
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->NavAgentProps.bCanFly = false;
	GetCharacterMovement()->NavAgentProps.bCanJump = true;
	GetCharacterMovement()->NavAgentProps.bCanWalk = true;
	GetCharacterMovement()->NavAgentProps.bCanSwim = false;

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
	HandleCameraTransforms(DeltaTime);
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


void AVincentBloodberry::HandleHeadBob(float DeltaTime)
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


void AVincentBloodberry::HandleCameraTransforms(float DeltaTime)
{
	const FRotator CurrentControlRotation = GetController()->GetControlRotation();
	const float CalculatedCameraRoll = CurrentCameraTiltRoll + CurrentCameraLeanRoll;
	const FRotator CalculatedCameraRotation(CurrentControlRotation.Pitch, CurrentControlRotation.Yaw, CalculatedCameraRoll);

	const FVector CurrentCameraCollLocation = GetHead()->GetRelativeLocation();
	const float CameraCollLocationHeightTarget = (GetCharacterMovement()->IsCrouching()) ?
		CameraCollisionCrouchLocation.Z : CameraCollisionWalkLocation.Z;
	const float InterpCameraCollHeight = FMath::FInterpTo(CurrentCameraCollLocation.Z, CameraCollLocationHeightTarget, DeltaTime, CrouchSpeed);
	const FVector CalculatedCameraCollLocation(CurrentCameraCollLocation.X, CurrentCameraLeanY, InterpCameraCollHeight);

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

	GetCharacterMovement()->MaxWalkSpeed = (bIsRunning) ?
		MovementDataMap[CurrentMovementState].FastMoveSpeed : MovementDataMap[CurrentMovementState].MoveSpeed;

	bCanMove = MovementDataMap[CurrentMovementState].bCanMove;
	bCanLean = MovementDataMap[CurrentMovementState].bCanLean;
	bCanCrouch = MovementDataMap[CurrentMovementState].bCanCrouch;
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
	
	if (!GetCharacterMovement()->IsCrouching())
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}


void AVincentBloodberry::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	UpdateMovementCharacteristics(EMovementState::Crouch);
	FVector HeadLoc = GetHead()->GetRelativeLocation();
	const float HeadHeightAdjust = CameraCollisionWalkLocation.Z - HeadLoc.Z;
	
	HeadLoc.Z = CameraCollisionWalkLocation.Z + (CameraCollisionWalkLocation.Z - CameraCollisionCrouchLocation.Z) - HeadHeightAdjust;

	GetHead()->SetRelativeLocation(FVector(0.f)); // COSTYL
	GetHead()->SetRelativeLocation(HeadLoc);
}


void AVincentBloodberry::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	UpdateMovementCharacteristics(EMovementState::Walk);
	FVector HeadLoc = GetHead()->GetRelativeLocation();
	const float HeadHeightAdjust = CameraCollisionCrouchLocation.Z - HeadLoc.Z;

	HeadLoc.Z = CameraCollisionCrouchLocation.Z - (CameraCollisionWalkLocation.Z - CameraCollisionCrouchLocation.Z) - HeadHeightAdjust;

	GetHead()->SetRelativeLocation(FVector(0.f)); // COSTYL
	GetHead()->SetRelativeLocation(HeadLoc);
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