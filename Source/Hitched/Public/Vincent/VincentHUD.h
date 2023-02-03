/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "VincentHUD.generated.h"

/**
 * 
 */
UCLASS()
class HITCHED_API AVincentHUD : public AHUD
{
	GENERATED_BODY()

public:

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	void UpdateTexture(UTexture2D* NewTexture);

protected:

	UPROPERTY(VisibleAnywhere, Category = "Texture")
	UTexture2D* LightGemTex = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> LightGemWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
	UUserWidget* CurrentWidget = nullptr;

private:

	AVincentHUD();
};