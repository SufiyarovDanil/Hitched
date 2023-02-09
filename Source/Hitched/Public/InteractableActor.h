/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interact.h"
#include "InteractableActor.generated.h"


UCLASS()
class HITCHED_API AInteractableActor : public AActor, public IInteract
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	AInteractableActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region INTERACT_INTERFACE

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(AActor* Caller);
	virtual void Interact_Implementation(AActor* Caller);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void StartFocus();
	virtual void StartFocus_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void EndFocus();
	virtual void EndFocus_Implementation();

#pragma endregion

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
