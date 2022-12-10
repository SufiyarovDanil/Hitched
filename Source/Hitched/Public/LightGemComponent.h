// Fill out your copyright notice in the Description page of Project Settings.

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

	float GetBrightness() const { return BrightnessOutput; }

	UFUNCTION(BlueprintCallable)
	UTextureRenderTarget2D* GetTopTexture() const { return RenderTargetTop; }

protected:

	UPROPERTY()
	UStaticMeshComponent* Octahedron = nullptr;

	UPROPERTY()
	USceneCaptureComponent2D* SceneCaptureTop = nullptr;

	UPROPERTY()
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

	UPROPERTY()
	UTextureRenderTarget2D* RenderTargetTop = nullptr;

	UPROPERTY()
	UTextureRenderTarget2D* RenderTargetBottom = nullptr;

	FRenderCommandFence CaptureFenceTop;

	FRenderCommandFence CaptureFenceBottom;

	UPROPERTY()
	float LightUpdateInterval;

	UPROPERTY()
	float BrightnessOutput;

#pragma endregion

};
