/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Action.generated.h"


UENUM(BlueprintType)
enum class EActionType : uint8
{
	Interacting	UMETA(DisplayName = "Interact"),
	Grabbing	UMETA(DisplayName = "Grab")
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAction : public UInterface
{
	GENERATED_BODY()
};


/**
 * 
 */
class HITCHED_API IAction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void DoAction(AActor* Caller);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StartFocus();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void EndFocus();
};
