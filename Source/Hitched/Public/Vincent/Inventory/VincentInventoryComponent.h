/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VincentInventoryComponent.generated.h"


class AVincentBloodberry;
class AInventory;
//struct FHitResult;


UENUM(BlueprintType)
enum class EInventoryState : uint8
{
	Opened		UMETA(DisplayName = "Opened"),
	OnOpening	UMETA(DisplayName = "On Opening"),
	Closed		UMETA(DisplayName = "Closed"),
	OnClosing	UMETA(DisplayName = "On Closing")
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HITCHED_API UVincentInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory State")
	EInventoryState CurrentInventoryState;

protected:

	UPROPERTY(VisibleAnywhere, Category = "Owning Character")
	AVincentBloodberry* OwningCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Case Mesh")
	AInventory* InventoryCase = nullptr;

private:

	UPROPERTY(VisibleAnywhere, Category = "Params | Trace")
	float TraceForwardLength;

	UPROPERTY(VisibleAnywhere, Category = "Params")
	float AllowedFloorAngle;

	UPROPERTY(VisibleAnywhere, Category = "Params | Time")
	float CasePlacingTime;

	UPROPERTY()
	float CasePlacingProgressTime;

	UPROPERTY()
	float CaseClosingAnimTime;

	UPROPERTY()
	float CaseClosingAnimProgressTime;

	UPROPERTY()
	FVector CasePlaceLocation;

public:

	// Sets default values for this component's properties
	UVincentInventoryComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*  */
	void ToggleButtonPressed();

	bool IsClosed() const { return CurrentInventoryState == EInventoryState::Closed; }

	EInventoryState GetCurrentState() const { return CurrentInventoryState; }

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

private:

	void PlaceCase();

	void HideCase();

	void TickPlacingCase(const float DeltaTime);

	void TickHidingCase(const float DeltaTime);

	bool CanPlaceCase();

	bool CanPlaceToHit(FHitResult& Hit, FCollisionShape& CaseCollision);

};
