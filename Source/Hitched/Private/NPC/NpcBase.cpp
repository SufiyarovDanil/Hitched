/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "NPC/NpcBase.h"


// Sets default values
ANpcBase::ANpcBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HairMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	HairMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	ClothMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Cloth"));
	ClothMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}


// Called when the game starts or when spawned
void ANpcBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (HairMesh)
	{
		HairMesh->SetMasterPoseComponent(GetMesh());
	}

	if (ClothMesh)
	{
		ClothMesh->SetMasterPoseComponent(GetMesh());
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
