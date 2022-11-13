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

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(45.5f, 91.f); // Radius and half heigh, so we need to divide Vincent's height by 2 (182 / 2)

	// Set head parameters
	HeadOriginLocation = FVector(0.f, 0.f, 60.f);
	HeadOriginRotation = FRotator(0.f, 0.f, 0.f);

	Head = CreateDefaultSubobject<USphereComponent>(TEXT("Head"));
	Head->SetupAttachment(GetCapsuleComponent());
	Head->SetRelativeLocation(HeadOriginLocation);
	Head->SetSphereRadius(20.f);

	// Set camera parameters
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(Head);
	//FirstPersonCamera->SetRelativeLocation(-100.f, 0.f, 0.f);
	FirstPersonCamera->SetFieldOfView(100.f);
	FirstPersonCamera->bUsePawnControlRotation = true;

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
	//GetCharacterMovement()->bOrientRotationToMovement = true; // never uncomment this shit
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->AirControl = 0.f;
	GetCharacterMovement()->MaxWalkSpeed = 111.f;
	GetCharacterMovement()->SetWalkableFloorAngle(60);
	GetCharacterMovement()->MaxAcceleration = 1024.f;				// Inertia movement param
	GetCharacterMovement()->BrakingFrictionFactor = 0.f;			// Inertia movement param
	GetCharacterMovement()->GroundFriction = 4.f;					// Inertia movement param
	GetCharacterMovement()->BrakingDecelerationWalking = 512.f;		// Inertia movement param
	GetCharacterMovement()->PerchRadiusThreshold = 20.f;
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
}

// Called every frame
void AVincentBloodberry::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleHeadBob(DeltaTime);
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
	const bool bCanHeadBob = !GetCharacterMovement()->IsFalling() && Velocity > 0.f;
	
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
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Head bob stopped"), Velocity));
			HeadBobTAnim.Stop();
		}
	}
	HeadBobTAnim.TickTimeline(Velocity * DeltaTime);
}

void AVincentBloodberry::MoveForward(float Scale)
{
	if (Scale != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Scale);
	}
}

void AVincentBloodberry::MoveRight(float Scale)
{
	if (Scale != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Scale);
	}
}

void AVincentBloodberry::OnLeaning(float Scale)
{
	const float DeltaTime = GetWorld()->DeltaTimeSeconds;

	const FVector HeadCurrentLocation = GetHead()->GetRelativeLocation();
	const FVector LeanTargetLocation = HeadOriginLocation + FVector(0.f, LeanDistance * Scale, 0.f);
	const FVector InterpolatedHeadLocation = FMath::VInterpTo(HeadCurrentLocation, LeanTargetLocation, DeltaTime, LeanSpeed);

	const FRotator HeadCurrentRotation = GetController()->GetControlRotation();
	const FRotator LeanTargetRotation = FRotator(HeadCurrentRotation.Pitch, HeadCurrentRotation.Yaw, LeanRotation * Scale);
	const FRotator InterpolatedCharacterRotation = FMath::RInterpTo(HeadCurrentRotation, LeanTargetRotation, DeltaTime, LeanSpeed);

	GetHead()->SetRelativeLocation(InterpolatedHeadLocation);
	GetController()->SetControlRotation(InterpolatedCharacterRotation);
}

void AVincentBloodberry::StartRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 208.f;
}

void AVincentBloodberry::StopRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 111.f;
}

void AVincentBloodberry::HeadBobTAnimProgress()
{
	const float TimelineValue = HeadBobTAnim.GetPlaybackPosition();
	const FVector TimelineLoc = HeadBobCurve->GetVectorValue(TimelineValue);

	FirstPersonCamera->SetRelativeLocation(TimelineLoc);
}

void AVincentBloodberry::MakeFootstep()
{
	if (FootStepSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FootStepSound, GetActorLocation());
	}
}