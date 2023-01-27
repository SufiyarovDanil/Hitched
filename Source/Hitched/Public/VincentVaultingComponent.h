/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VincentVaultingComponent.generated.h"


class AVincentBloodberry;
class UCapsuleComponent;
struct FHitResult;

/* Sub enum of Vaulting in EMovementState */
UENUM(BlueprintType)
enum class EVaultState : uint8
{
	NotVaulting		UMETA(DisplayName = "Not Vaulting"),
	Vaulting		UMETA(DisplayName = "Vaulting")
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HITCHED_API UVincentVaultingComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	UVincentVaultingComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

private:

#pragma region VARIABLES

	AVincentBloodberry* OwningCharacter = nullptr;

	EVaultState VaultState;

	FVector EndingLocation;

	FVector StartingLocation;

	float MinVaultingHeight;

	float MaxVaultingHeight;

	float Progress;

	float VaultSpeed;

#pragma endregion

#pragma region FUNCTIONS

public:

	bool CanVault();

	void Vault();

private:

	void TickVault(const float DeltaTime);

	bool CanVaultToHit(const FHitResult& HitResult);

	bool CheckCapsuleCollision(const FVector Center, const float HalfHeight, const float Radius);

#pragma endregion

};
