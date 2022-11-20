/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightGem.generated.h"


class USceneCaptureComponent2D;

UCLASS()
class HITCHED_API ALightGem : public AActor
{
	GENERATED_BODY()
	
public:	

	/* Returns the light level */
	float GetLightScale() const { return LightLevel; }

protected:

	/* A static octahedron mesh that will capture the light falling on it */
	UPROPERTY(VisibleAnywhere, Category = "Octahedron Static Mesh")
	UStaticMeshComponent* Octahedron = nullptr;

	/* Capture and render top of octahedron */
	UPROPERTY(VisibleAnywhere, Category = "Octahedron Snapshot")
	USceneCaptureComponent2D* OctahedronTopCapture = nullptr;

	/* Capture and render bottom of octahedron */
	UPROPERTY(VisibleAnywhere, Category = "Octahedron Snapshot")
	USceneCaptureComponent2D* OctahedronBottomCapture = nullptr;

private:

#pragma region FUNCTIONS

	/* Sets default values for this actor's properties */
	ALightGem();

	/* Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/* Called every frame */
	virtual void Tick(float DeltaTime) override;

	/* Handling LightLevel where light gem located. Calling every frame */
	void HandleLightLevel();

	/* Finds brightest pixel of texture */
	float AnalyzeTexture(UTextureRenderTarget2D* TextureTarget);

#pragma endregion

#pragma region VARIABLES

	/* Indicator of Character's visiblity. Scaled from 0 to 1 */
	UPROPERTY(VisibleAnywhere, Category = "Light Level")
	float LightLevel = 0.f;

#pragma endregion
};
