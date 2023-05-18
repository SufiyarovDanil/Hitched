/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "NPC/EnemyNpc.h"
#include "NPC/AI/EnemyAIController.h"


AEnemyNpc::AEnemyNpc()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, 270.f, 0.f));
	GetMesh()->SetRelativeScale3D(FVector(0.5f));

	// Init animation
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackAnimMontageAsset(
		TEXT("AnimMontage'/Game/NPC/Male/Animations/AttackMontage.AttackMontage'"));

	if (AttackAnimMontageAsset.Succeeded())
	{
		AttackAnimMontage = AttackAnimMontageAsset.Object;
	}

	// Init manequin
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MaleMeshAsset(
		TEXT("SkeletalMesh'/Game/NPC/Male/Meshes/MaleManequin.MaleManequin'"));

	if (MaleMeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MaleMeshAsset.Object);
	}

	// Init animation
    static ConstructorHelpers::FObjectFinder<UClass> AnimBPAsset(
		TEXT("Class'/Game/NPC/Male/Animations/EnemyNPCAnimBP.EnemyNPCAnimBP_C'"));

	if (AnimBPAsset.Succeeded())
	{
		GetMesh()->SetAnimationMode(EAnimationMode::Type::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(AnimBPAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HairMeshAsset(
		TEXT("SkeletalMesh'/Game/NPC/Male/Meshes/Outfits/Hairs/WorkerHat.WorkerHat'"));

	if (HairMeshAsset.Succeeded())
	{
		HairMesh->SetSkeletalMesh(HairMeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TopClothMeshAsset(
		TEXT("SkeletalMesh'/Game/NPC/Male/Meshes/Outfits/Top/WorkerJacket.WorkerJacket'"));

	if (TopClothMeshAsset.Succeeded())
	{
		TopClothMesh->SetSkeletalMesh(TopClothMeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BottomClothMeshAsset(
		TEXT("SkeletalMesh'/Game/NPC/Male/Meshes/Outfits/Bottom/WorkerPants.WorkerPants'"));

	if (BottomClothMeshAsset.Succeeded())
	{
		BottomClothMesh->SetSkeletalMesh(BottomClothMeshAsset.Object);
	}

	AIControllerClass = AEnemyAIController::StaticClass();
}


void AEnemyNpc::Attack()
{
	if (!AttackAnimMontage)
	{
		return;
	}

	if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackAnimMontage))
	{
		return;
	}

	GetMesh()
		->AnimScriptInstance
		->Montage_Play(AttackAnimMontage);
}
