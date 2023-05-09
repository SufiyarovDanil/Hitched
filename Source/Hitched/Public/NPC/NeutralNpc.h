/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#pragma once

#include "CoreMinimal.h"
#include "NPC/NpcBase.h"
#include "Interfaces/Action.h"
#include "NeutralNpc.generated.h"


class USoundCue;


/**
 * 
 */
UCLASS()
class HITCHED_API ANeutralNpc : public ANpcBase, public IAction
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, Category = "State")
	bool bIsTalking;

protected:

	/*  */
	UPROPERTY(EditDefaultsOnly, Category = "")
	USoundCue* Replicas = nullptr;

	/*  */
	UPROPERTY(VisibleAnywhere, Category = "Outfit")
	TArray<USkeletalMesh*> Hairs;

	/*  */
	UPROPERTY(VisibleAnywhere, Category = "Outfit")
	TArray<USkeletalMesh*> TopClothes;

	/*  */
	UPROPERTY(VisibleAnywhere, Category = "Outfit")
	TArray<USkeletalMesh*> BottomClothes;

private:

	void StartTalking();

	void EndTalking();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	ANeutralNpc();

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

};
