/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "Vincent/VincentBloodberry.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/FootstepComponent.h"
#include "Curves/CurveVector.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Vincent/LightGemComponent.h"
#include "Vincent/VincentMovementComponent.h"
#include "Vincent/VincentVaultingComponent.h"
#include "Vincent/VincentLeaningComponent.h"
#include "Vincent/Inventory/VincentInventoryComponent.h"
#include "Vincent/LandImpactComponent.h"
#include "Vincent/VincentActioningComponent.h"
#include "Weapons/WeaponBase.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AVincentBloodberry::AVincentBloodberry(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UVincentMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Init movement properties
	MovementComp = Cast<UVincentMovementComponent>(ACharacter::GetMovementComponent());
	InitMovementCharacteristics();
	CurrentMovementState = EMovementState::Walk;

	// Init vaulting component
	VaultingComp = CreateDefaultSubobject<UVincentVaultingComponent>(TEXT("Vaulting"));

	// Init footstep component
	FootstepComp = CreateDefaultSubobject<UFootstepComponent>(TEXT("Footstep"));

	// Init Inventory Component
	InventoryComp = CreateDefaultSubobject<UVincentInventoryComponent>(TEXT("Inventory"));

	// Init interacting component
	ActioningComp = CreateDefaultSubobject<UVincentActioningComponent>(TEXT("Actioning"));

	// Divided Vincent's height by 2 (182 / 2)
	CapsuleHalfHeight = 91.f;
	CrouchCapsuleHalfHeight = MovementComp->CrouchedHalfHeight;
	CrouchSpeed = 10.f;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(45.5f, CapsuleHalfHeight);

	// Set head parameters
	const float CameraCollisionRadius = 30.f;
	CameraCollisionWalkLocation = FVector(0.f, 0.f, CapsuleHalfHeight - CameraCollisionRadius);
	CameraCollisionCrouchLocation = FVector(0.f, 0.f, CrouchCapsuleHalfHeight - CameraCollisionRadius);
	CameraCollisionDefaulRotation = FRotator(0.f, 0.f, 0.f);

	// Init leaning component
	LeaningComp = CreateDefaultSubobject<UVincentLeaningComponent>(TEXT("Leaning"));
	LeaningComp->SetupAttachment(GetCapsuleComponent());
	LeaningComp->SetRelativeLocation(CameraCollisionWalkLocation);

	CameraCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Head"));
	CameraCollision->SetupAttachment(LeaningComp);
	CameraCollision->SetSphereRadius(CameraCollisionRadius);

	// Land Impact Init
	LandImpactComp = CreateDefaultSubobject<ULandImpactComponent>(TEXT("Land Timeline"));
	LandImpactComp->SetupAttachment(CameraCollision);

	// Set camera parameters
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	Camera->SetupAttachment(LandImpactComp);
	Camera->SetFieldOfView(100.f);
	Camera->bUsePawnControlRotation = true;

	// Init left hand
	LeftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Left Hand"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> LeftHandAsset(
		TEXT("SkeletalMesh'/Game/Vincent/Meshes/LeftHand.LeftHand'"));

	if (LeftHandAsset.Succeeded())
	{
		LeftHand->SetSkeletalMesh(LeftHandAsset.Object);
	}

	LeftHand->SetupAttachment(Camera);
	LeftHand->SetCastShadow(false);

	static ConstructorHelpers::FObjectFinder<UClass> LeftHandAnimBPAsset(
		TEXT("Class'/Game/Vincent/Animations/LeftHandAnimBP.LeftHandAnimBP_C'"));

	if (LeftHandAnimBPAsset.Succeeded())
	{
		LeftHand->SetAnimationMode(EAnimationMode::Type::AnimationBlueprint);
		LeftHand->SetAnimInstanceClass(LeftHandAnimBPAsset.Object);
	}

	bIsLeftHandDrawing = false;

	// Init right hand
	RightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Right Hand"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> RightHandAsset(
		TEXT("SkeletalMesh'/Game/Vincent/Meshes/RightHand.RightHand'"));

	if (RightHandAsset.Succeeded())
	{
		RightHand->SetSkeletalMesh(RightHandAsset.Object);
	}

	RightHand->SetupAttachment(Camera);
	RightHand->SetCastShadow(false);

	static ConstructorHelpers::FObjectFinder<UClass> RightHandAnimBPAsset(
		TEXT("Class'/Game/Vincent/Animations/RightHandAnimBP.RightHandAnimBP_C'"));

	if (RightHandAnimBPAsset.Succeeded())
	{
		RightHand->SetAnimationMode(EAnimationMode::Type::AnimationBlueprint);
		RightHand->SetAnimInstanceClass(RightHandAnimBPAsset.Object);
	}

	bIsRightHandDrawing = false;

	// Init lightgem component
	LightGem = CreateDefaultSubobject<ULightGemComponent>(TEXT("Light Gem"));
	LightGem->SetupAttachment(CameraCollision);

	// When character yaw rotating, light gem output value changes. That's why we need to lock light gem rotation
	LightGem->SetUsingAbsoluteRotation(true);

	// Head bob curve init
	static ConstructorHelpers::FObjectFinder<UCurveVector> HeadBobCurveAsset(
		TEXT("CurveVector'/Game/Vincent/TimelineAnims/HeadBobCurve.HeadBobCurve'"));

	if (HeadBobCurveAsset.Succeeded())
	{
		HeadBobCurve = HeadBobCurveAsset.Object;
	}

	bIsPaused = false;
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
		UpdateMovementCharacteristics(EMovementState::Walk);
	}
}


// Called every frame
void AVincentBloodberry::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleHeadBob(DeltaTime);
	HandleCameraTransforms(DeltaTime);

	if (GEngine && LightGem)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("Light level: %f"), LightGem->GetBrightness()));
	}
}


// Called to bind functionality to input
void AVincentBloodberry::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Mouse things
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AVincentBloodberry::StartFiring);

	// Action input
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AVincentBloodberry::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AVincentBloodberry::StopJumping);

	PlayerInputComponent->BindAction("Toggle Crouch", IE_Pressed, this, &AVincentBloodberry::ToggleCrouch);
	PlayerInputComponent->BindAction("Do Action", IE_Released, this, &AVincentBloodberry::DoAction);
	PlayerInputComponent->BindAction("Toggle Show Watch", IE_Pressed, this, &AVincentBloodberry::ToggleShowWatch);
	PlayerInputComponent->BindAction("Toggle Draw Right Hand", IE_Pressed, this, &AVincentBloodberry::ToggleDrawRightHand);
	PlayerInputComponent->BindAction("Lean Left", IE_Pressed, this, &AVincentBloodberry::LeanLeft);
	PlayerInputComponent->BindAction("Lean Left", IE_Released, this, &AVincentBloodberry::UnleanLeft);
	PlayerInputComponent->BindAction("Lean Right", IE_Pressed, this, &AVincentBloodberry::LeanRight);
	PlayerInputComponent->BindAction("Lean Right", IE_Released, this, &AVincentBloodberry::UnleanRight);

	PlayerInputComponent->BindAction("Toggle Inventory", IE_Pressed, this, &AVincentBloodberry::ToggleInventory);
	PlayerInputComponent->BindAction("Next Item", IE_Pressed, InventoryComp, &UVincentInventoryComponent::NextItem);
	PlayerInputComponent->BindAction("Prev Item", IE_Pressed, InventoryComp, &UVincentInventoryComponent::PrevItem);
	PlayerInputComponent->BindKey(EKeys::Enter, IE_Pressed, this, &AVincentBloodberry::TakeSelectedItemFromInventory);
	PlayerInputComponent->BindKey(EKeys::R, IE_Pressed, this, &AVincentBloodberry::ReloadWeapon);

	// Movement input
	PlayerInputComponent->BindAxis("Move Forward", this, &AVincentBloodberry::MoveForward);
	PlayerInputComponent->BindAxis("Move Right", this, &AVincentBloodberry::MoveRight);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AVincentBloodberry::StartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AVincentBloodberry::StopRunning);

	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AVincentBloodberry::TogglePause).bExecuteWhenPaused = true;
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
	const FVector CurrentCameraCollLocation = GetHead()->GetRelativeLocation();
	const float CameraCollLocationHeightTarget = (GetCharacterMovement()->IsCrouching()) ?
		CameraCollisionCrouchLocation.Z : CameraCollisionWalkLocation.Z;
	const float InterpCameraCollHeight = FMath::FInterpTo(CurrentCameraCollLocation.Z, CameraCollLocationHeightTarget, DeltaTime, CrouchSpeed);
	const FVector CalculatedCameraCollLocation(CurrentCameraCollLocation.X, CurrentCameraCollLocation.Y, InterpCameraCollHeight);

	GetHead()->SetRelativeLocation(CalculatedCameraCollLocation);
}


void AVincentBloodberry::UpdateMovementCharacteristics(EMovementState NewMovementState)
{
	if (!MovementDataMap.Contains(NewMovementState))
	{
		return;
	}

	CurrentMovementState = NewMovementState;

	if (bIsRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = MovementDataMap[CurrentMovementState].FastMoveSpeed;
		GetCharacterMovement()->MaxWalkSpeedCrouched = 120.f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = MovementDataMap[CurrentMovementState].MoveSpeed;
		GetCharacterMovement()->MaxWalkSpeedCrouched = 80.f;
	}

	bCanMove = MovementDataMap[CurrentMovementState].bCanMove;
	bCanLean = MovementDataMap[CurrentMovementState].bCanLean;
	bCanCrouch = MovementDataMap[CurrentMovementState].bCanCrouch;
}


void AVincentBloodberry::MoveForward(float Scale)
{
	if (Scale != 0.0f
		&& bCanMove
		&& !VaultingComp->IsVaulting()
		&& InventoryComp->IsClosed())
	{
		AddMovementInput(GetActorForwardVector(), Scale);
	}
}


void AVincentBloodberry::MoveRight(float Scale)
{
	if (Scale != 0.0f
		&& bCanMove
		&& !VaultingComp->IsVaulting()
		&& InventoryComp->IsClosed())
	{
		AddMovementInput(GetActorRightVector(), Scale);
	}
}


void AVincentBloodberry::StartRunning()
{
	if (!bCanMove)
	{
		return;
	}

	bIsRunning = true;
	UpdateMovementCharacteristics(CurrentMovementState);
}


void AVincentBloodberry::StopRunning()
{
	if (!bCanMove)
	{
		return;
	}

	bIsRunning = false;
	UpdateMovementCharacteristics(CurrentMovementState);
}


void AVincentBloodberry::ToggleCrouch()
{
	if (!bCanCrouch
		|| GetCharacterMovement()->IsFalling()
		|| VaultingComp->IsVaulting()
		|| !InventoryComp->IsClosed())
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


void AVincentBloodberry::DoAction()
{
	if (!ActioningComp
		|| !InventoryComp
		|| !InventoryComp->IsClosed())
	{
		return;
	}

	ActioningComp->ActionButtonPressed();
}


void AVincentBloodberry::ToggleShowWatch()
{
	if (VaultingComp->IsVaulting() || !InventoryComp->IsClosed())
	{
		return;
	}

	bIsLeftHandDrawing = !bIsLeftHandDrawing;
}


void AVincentBloodberry::ToggleDrawRightHand()
{
	if (VaultingComp->IsVaulting()
		|| !InventoryComp->IsClosed()
		|| !CurrentWeapon)
	{
		return;
	}

	if (bIsRightHandDrawing)
	{
		CurrentWeapon->Unequip();
	}
	else
	{
		CurrentWeapon->Equip();
	}

	bIsRightHandDrawing = !bIsRightHandDrawing;
}


void AVincentBloodberry::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	JumpEndPoint = GetActorLocation();

	if (!LandImpactComp || !FootstepComp)
	{
		return;
	}

	LandImpactComp->StartAnimation();
	FootstepComp->OnLanded();
}


/**
* Original implementation of this function won't let
* us using jump when character is crouching.
* I erased several rows for correct work.
*/
bool AVincentBloodberry::CanJumpInternal_Implementation() const
{
	// Ensure that the CharacterMovement state is valid
	bool bCanJump = GetCharacterMovement()->CanAttemptJump();

	if (bCanJump)
	{
		// Ensure JumpHoldTime and JumpCount are valid.
		if (!bWasJumping || GetJumpMaxHoldTime() <= 0.0f)
		{
			if (JumpCurrentCount == 0 && GetCharacterMovement()->IsFalling())
			{
				bCanJump = JumpCurrentCount + 1 < JumpMaxCount;
			}
			else
			{
				bCanJump = JumpCurrentCount < JumpMaxCount;
			}
		}
		else
		{
			// Only consider JumpKeyHoldTime as long as:
			// A) The jump limit hasn't been met OR
			// B) The jump limit has been met AND we were already jumping
			const bool bJumpKeyHeld = (bPressedJump && JumpKeyHoldTime < GetJumpMaxHoldTime());
			bCanJump = bJumpKeyHeld &&
				((JumpCurrentCount < JumpMaxCount) || (bWasJumping && JumpCurrentCount == JumpMaxCount));
		}
	}

	return bCanJump;
}


void AVincentBloodberry::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	UpdateMovementCharacteristics(EMovementState::Crouch);
	FVector NewHeadLoc = GetHead()->GetRelativeLocation();
	const float HeadHeightAdjust = CameraCollisionWalkLocation.Z - NewHeadLoc.Z;

	NewHeadLoc.Z = CameraCollisionWalkLocation.Z + (CameraCollisionWalkLocation.Z - CameraCollisionCrouchLocation.Z) - HeadHeightAdjust;

	GetHead()->SetRelativeLocation(FVector(0.f)); // COSTYL
	GetHead()->SetRelativeLocation(NewHeadLoc);
}


void AVincentBloodberry::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	UpdateMovementCharacteristics(EMovementState::Walk);
	FVector NewHeadLoc = GetHead()->GetRelativeLocation();
	const float HeadHeightAdjust = CameraCollisionCrouchLocation.Z - NewHeadLoc.Z;

	NewHeadLoc.Z = CameraCollisionCrouchLocation.Z - (CameraCollisionWalkLocation.Z - CameraCollisionCrouchLocation.Z) - HeadHeightAdjust;

	GetHead()->SetRelativeLocation(FVector(0.f)); // COSTYL
	GetHead()->SetRelativeLocation(NewHeadLoc);
}


void AVincentBloodberry::Jump()
{
	if (!InventoryComp || !InventoryComp->IsClosed())
	{
		return;
	}

	if (VaultingComp)
	{
		if (VaultingComp->CanVault())
		{
			VaultingComp->Vault();

			return;
		}
	}

	JumpStartPoint = GetActorLocation();
	Super::Jump();
}


void AVincentBloodberry::LeanLeft()
{
	if (!LeaningComp)
	{
		return;
	}

	LeaningComp->SetLeanSide(ELeanState::Left, true);
}


void AVincentBloodberry::UnleanLeft()
{
	if (!LeaningComp)
	{
		return;
	}

	LeaningComp->SetLeanSide(ELeanState::Left, false);
}


void AVincentBloodberry::LeanRight()
{
	if (!LeaningComp)
	{
		return;
	}

	LeaningComp->SetLeanSide(ELeanState::Right, true);
}


void AVincentBloodberry::UnleanRight()
{
	if (!LeaningComp)
	{
		return;
	}

	LeaningComp->SetLeanSide(ELeanState::Right, false);
}


void AVincentBloodberry::StartFiring()
{
	if (!CurrentWeapon || !bIsRightHandDrawing)
	{
		return;
	}

	CurrentWeapon->StartFire();
}


void AVincentBloodberry::ReloadWeapon()
{
	if (!CurrentWeapon || !bIsRightHandDrawing)
	{
		return;
	}

	CurrentWeapon->Reload();
}


void AVincentBloodberry::ToggleInventory()
{
	FHitResult Hit; // Just a plug

	if (!InventoryComp
		|| !GetCharacterMovement()->IsMovingOnGround()
		|| VaultingComp->IsVaulting())
	{
		return;
	}

	if (InventoryComp->IsClosed())
	{
		if (!InventoryComp->CanPlaceCase(Hit))
		{
			return;
		}
	}

	if (!GetCharacterMovement()->IsCrouching())
	{
		Crouch();
	}

	InventoryComp->ToggleButtonPressed();
}


void AVincentBloodberry::TakeSelectedItemFromInventory()
{
	if (!InventoryComp
		|| (InventoryComp->GetCurrentState() != EInventoryState::Opened))
	{
		return;
	}

	if (CurrentWeapon)
	{
		InventoryComp->AddItem(CurrentWeapon);
	}

	DetachWeaponFromHand();

	CurrentWeapon = InventoryComp->TakeSelectedItem();

	AttachWeaponToHand();
}


void AVincentBloodberry::TogglePause()
{
	UGameplayStatics::SetGamePaused(GetWorld(), !bIsPaused);

	bIsPaused = !bIsPaused;
}


void AVincentBloodberry::AttachWeaponToHand()
{
	if (!RightHand || !CurrentWeapon)
	{
		return;
	}
	
	CurrentWeapon->AttachToComponent(RightHand, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
	CurrentWeapon->SetActorRelativeLocation(CurrentWeapon->GetAttachLocation());
	CurrentWeapon->SetActorRelativeRotation(CurrentWeapon->GetAttachRotation());
}


void AVincentBloodberry::DetachWeaponFromHand()
{
	if (!CurrentWeapon)
	{
		return;
	}

	CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentWeapon->SetActorRelativeLocation(FVector(0.f));
	CurrentWeapon->SetActorRelativeRotation(FRotator(0.f));
}


void AVincentBloodberry::HeadBobTAnimProgress()
{
	const float TimelineValue = HeadBobTAnim.GetPlaybackPosition();
	const FVector TimelineLoc = HeadBobCurve->GetVectorValue(TimelineValue);

	Camera->SetRelativeLocation(TimelineLoc);
}


void AVincentBloodberry::MakeFootstep()
{
	if (!FootstepComp)
	{
		return;
	}

	FootstepComp->MakeFootstep();
}


float AVincentBloodberry::GetLightLevel()
{
	if (!LightGem)
	{
		return 0.f;
	}
	
	return LightGem->GetBrightness();
}


float AVincentBloodberry::PlayAnimMontage(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{

	if (!AnimMontage || !RightHand || !RightHand->AnimScriptInstance)
	{
		return 0.f;
	}

	return RightHand->AnimScriptInstance->Montage_Play(AnimMontage, InPlayRate);
}


 void AVincentBloodberry::StopAnimMontage(UAnimMontage* AnimMontage)
 {
	if (!AnimMontage ||!RightHand ||!RightHand->AnimScriptInstance)
	{
		return;
	}

	RightHand->AnimScriptInstance->Montage_Stop(AnimMontage->BlendOut.GetBlendTime(), AnimMontage);
 }


void AVincentBloodberry::InitMovementCharacteristics()
{
	FMovementCharacteristics MovementWalking{};
	MovementWalking.MoveSpeed = 150.f;
	MovementWalking.FastMoveSpeed = 310.f;
	MovementWalking.bCanMove = true;
	MovementWalking.bCanLean = true;
	MovementWalking.bCanCrouch = true;

	FMovementCharacteristics MovementCrouching{};
	MovementCrouching.MoveSpeed = 60.f;
	MovementCrouching.FastMoveSpeed = 120.f;
	MovementCrouching.bCanMove = true;
	MovementCrouching.bCanLean = true;
	MovementCrouching.bCanCrouch = true;

	MovementDataMap.Add(EMovementState::Walk, MovementWalking);
	MovementDataMap.Add(EMovementState::Crouch, MovementCrouching);
}
