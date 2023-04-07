/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "Vincent/Inventory/Inventory.h"
#include "Components/BoxComponent.h"


// Sets default values
AInventory::AInventory()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InventoryMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Case"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> InventoryMeshAsset(
		TEXT("SkeletalMesh'/Game/Environment/Meshes/InventoryCase.InventoryCase'"));

	if (InventoryMeshAsset.Succeeded())
	{
		InventoryMesh->SetSkeletalMesh(InventoryMeshAsset.Object);
	}

	InventoryMesh->SetCastShadow(false);

	static ConstructorHelpers::FObjectFinder<UClass> CaseAnimBP(
		TEXT("Class'/Game/Environment/Animations/CaseAnimBP.CaseAnimBP_C'"));

	if (CaseAnimBP.Succeeded())
	{
		InventoryMesh->SetAnimationMode(EAnimationMode::Type::AnimationBlueprint);
		InventoryMesh->SetAnimInstanceClass(CaseAnimBP.Object);
	}

	RootComponent = InventoryMesh;
	bIsOpened = false;
}


// Called when the game starts or when spawned
void AInventory::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AInventory::OpenCase()
{
	bIsOpened = true;
}


void AInventory::CloseCase()
{
	bIsOpened = false;
}


void AInventory::AddToInventory()
{

}
