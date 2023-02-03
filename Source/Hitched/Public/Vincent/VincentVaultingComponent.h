/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VincentVaultingComponent.generated.h"


class AVincentBloodberry;
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

	UPROPERTY(VisibleAnywhere, Category = "Component Owner")
	AVincentBloodberry* OwningCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Vault State")
	EVaultState VaultState;

	UPROPERTY(VisibleAnywhere, Category = "Vaulting Start Location")
	FVector EndingLocation;

	UPROPERTY(VisibleAnywhere, Category = "Vaulting End Location")
	FVector StartingLocation;

	UPROPERTY(VisibleAnywhere, Category = "Min Vaulting Height")
	float MinVaultingHeight;

	UPROPERTY(VisibleAnywhere, Category = "Max Vaulting Height")
	float MaxVaultingHeight;

	UPROPERTY(VisibleAnywhere, Category = "Vaulting Progress")
	float Progress;

	UPROPERTY(VisibleAnywhere, Category = "Vaulting Speed")
	float VaultSpeed;

#pragma endregion

#pragma region FUNCTIONS

public:

	bool CanVault();

	bool IsVaulting();

	void Vault();

private:

	void TickVault(const float DeltaTime);

	bool CanVaultToHit(const FHitResult& HitResult);

	bool CheckCapsuleCollision(const FVector Center, const float HalfHeight, const float Radius);

#pragma endregion

};
