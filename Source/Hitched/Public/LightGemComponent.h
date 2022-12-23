/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LightGemComponent.generated.h"


class UTextureRenderTarget2D;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HITCHED_API ULightGemComponent : public USceneComponent
{
	GENERATED_BODY()

public:

	UFUNCTION()
	float GetBrightness() const { return BrightnessOutput; }

	UFUNCTION(BlueprintCallable)
	UTextureRenderTarget2D* GetTopTexture() const { return RenderTargetTop; }

protected:

	UPROPERTY(VisibleAnywhere, Category = "Ligt Receiver")
	UStaticMeshComponent* Octahedron = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Top Light Catcher")
	USceneCaptureComponent2D* SceneCaptureTop = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Bottom Light Catcher")
	USceneCaptureComponent2D* SceneCaptureBottom = nullptr;

private:

#pragma region FUNCTIONS

	// Sets default values for this component's properties
	ULightGemComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;

	void ComputeBrightness();

	float FindBrightestPixel(UTextureRenderTarget2D* RenderTarget);

#pragma endregion

#pragma region VARIABLES

	UPROPERTY(VisibleAnywhere, Category = "Top Snapshot")
	UTextureRenderTarget2D* RenderTargetTop = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Bottom Snapshot")
	UTextureRenderTarget2D* RenderTargetBottom = nullptr;

	FRenderCommandFence CaptureFenceTop;

	FRenderCommandFence CaptureFenceBottom;

	UPROPERTY(VisibleAnywhere, Category = "Tick Interval")
	float LightUpdateInterval;

	UPROPERTY(VisibleAnywhere, Category = "Light Level Output")
	float BrightnessOutput;

#pragma endregion

};
