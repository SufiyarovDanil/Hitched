/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Action.h"
#include "ActionableActorBase.generated.h"

UCLASS()
class HITCHED_API AActionableActorBase : public AActor, public IAction
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AActionableActorBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* */
	EActionType GetActionType() const { return ActionType; }

	/* */
	bool IsVaultable() const { return bIsVaultable; }

#pragma region INTERACT_INTERFACE

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void DoAction(AActor* Caller);
	virtual void DoAction_Implementation(AActor* Caller);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StartFocus();
	virtual void StartFocus_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void EndFocus();
	virtual void EndFocus_Implementation();

#pragma endregion

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Enum which determines how to interact with the actor */
	UPROPERTY(VisibleAnywhere, Category = "Action Type")
	EActionType ActionType;

private:

	/* */
	UPROPERTY(VisibleAnywhere, Category = "Vaulting")
	bool bIsVaultable;

};
