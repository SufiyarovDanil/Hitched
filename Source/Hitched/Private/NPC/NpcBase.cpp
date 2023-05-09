/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "NPC/NpcBase.h"
#include "Components\FootstepComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ANpcBase::ANpcBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HairMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	HairMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	HairMesh->SetupAttachment(GetMesh());

	TopClothMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Top Cloth"));
	TopClothMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	TopClothMesh->SetupAttachment(GetMesh());

	BottomClothMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bottom Cloth"));
	BottomClothMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	BottomClothMesh->SetupAttachment(GetMesh());

	// Init Footstep component
	FootstepComp = CreateDefaultSubobject<UFootstepComponent>(TEXT("Footstep component"));

	// Movement values
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	GetCharacterMovement()->SetWalkableFloorAngle(60);
	GetCharacterMovement()->MaxAcceleration = 1024.f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.f;
	GetCharacterMovement()->GroundFriction = 4.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 512.f;
	GetCharacterMovement()->PerchRadiusThreshold = 20.f;
}


// Called when the game starts or when spawned
void ANpcBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (HairMesh)
	{
		HairMesh->SetMasterPoseComponent(GetMesh());
	}

	if (TopClothMesh)
	{
		TopClothMesh->SetMasterPoseComponent(GetMesh());
	}

	if (BottomClothMesh)
	{
		BottomClothMesh->SetMasterPoseComponent(GetMesh());
	}
}


// Called every frame
void ANpcBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
void ANpcBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
