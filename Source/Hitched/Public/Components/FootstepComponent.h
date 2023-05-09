/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootstepComponent.generated.h"


class USoundCue;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HITCHED_API UFootstepComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, Category = "Data Map")
	TMap<UPhysicalMaterial*, USoundCue*> FootstepLoudSfxDataMap;

	UPROPERTY(VisibleAnywhere, Category = "Data Map")
	TMap<UPhysicalMaterial*, USoundCue*> FootstepQuietSfxDataMap;

	UPROPERTY(VisibleAnywhere, Category = "Data Map")
	TMap<UPhysicalMaterial*, USoundCue*> JumpSfxDataMap;

	UPROPERTY(VisibleAnywhere, Category = "Owning Pawn")
	ACharacter* OwningCharacter = nullptr;

private:

	UPhysicalMaterial* FindPhysMatOfFloor();

	void PlaySound(USoundCue* Sound);

	void DoLoudFootStep(UPhysicalMaterial* PhysMat);

	void DoQuietFootStep(UPhysicalMaterial* PhysMat);

	void DoLand(UPhysicalMaterial* PhysMat);

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

public:

	// Sets default values for this component's properties
	UFootstepComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void MakeFootstep();

	UFUNCTION(BlueprintCallable)
	void OnLanded();

};
