/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory.generated.h"


class APickableActor;
class UBoxComponent;


UCLASS()
class HITCHED_API AInventory : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsOpened;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	USkeletalMeshComponent* InventoryMesh = nullptr;

#pragma region ANIMATIONS

	UPROPERTY()
	UAnimMontage* OpenCaseAnim = nullptr;

	UPROPERTY()
	UAnimMontage* CloseCaseAnim = nullptr;

#pragma endregion

public:

	// Sets default values for this actor's properties
	AInventory();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddToInventory();

	void OpenCase();

	void CloseCase();

};
