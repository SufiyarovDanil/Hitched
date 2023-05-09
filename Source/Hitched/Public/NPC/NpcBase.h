/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NpcBase.generated.h"


class UFootstepComponent;


UCLASS()
class HITCHED_API ANpcBase : public ACharacter
{
	GENERATED_BODY()

protected:

	// Hair slot for customizing
	UPROPERTY(VisibleAnywhere, Category = "Outfit")
	USkeletalMeshComponent* HairMesh = nullptr;

	// Top cloth slot for customizing
	UPROPERTY(VisibleAnywhere, Category = "Outfit")
	USkeletalMeshComponent* TopClothMesh = nullptr;

	// Bottom cloth slot for customizing
	UPROPERTY(VisibleAnywhere, Category = "Outfit")
	USkeletalMeshComponent* BottomClothMesh = nullptr;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UFootstepComponent* FootstepComp = nullptr;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Sets default values for this character's properties
	ANpcBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
