/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "LightGemComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"


// Light gem texture render sizes
static constexpr uint32 LIGHTGEM_TEX_WIDTH = 32u; // 16u
static constexpr uint32 LIGHTGEM_TEX_HEIGHT = 32u; // 16u

// The brightness of octahedron's pixel is from 0 to 255.
// We multiplying current brightness and LIGHTGEM_SCALE for normalize the output value (from 0 to 1)
static constexpr float LIGHTGEM_SCALE = 1.f / 255.f;

// Coefficients by which to multiply the corresponding color values for the brightness formula
static constexpr float LIGHTGEM_R = 0.299f;
static constexpr float LIGHTGEM_G = 0.587f;
static constexpr float LIGHTGEM_B = 0.114f;

// Sets default values for this component's properties
ULightGemComponent::ULightGemComponent()
{
	BrightnessOutput = 0.f;
	LightUpdateInterval = 0.05f;

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = LightUpdateInterval;

	Octahedron = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Octahedron"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> OctahedronAsset(
		TEXT("StaticMesh'/Game/Mechanics/LightLevel/Octahedron.Octahedron'"));

	if (OctahedronAsset.Succeeded())
	{
		Octahedron->SetStaticMesh(OctahedronAsset.Object);
	}

	Octahedron->SetupAttachment(this);
	Octahedron->SetCastShadow(false);
	// Lightgem system is a backend process, so player should not see the octahedron
	Octahedron->SetVisibleInSceneCaptureOnly(true);

	SceneCaptureTop = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Top"));
	SceneCaptureTop->SetupAttachment(Octahedron);
	SceneCaptureTop->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 15.f), FRotator(-90.f, 0.f, 45.f));
	SceneCaptureTop->FOVAngle = 20.f;
	SceneCaptureTop->bCaptureEveryFrame = false;
	SceneCaptureTop->bCaptureOnMovement = false;

	SceneCaptureBottom = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Bottom"));
	SceneCaptureBottom->SetupAttachment(Octahedron);
	SceneCaptureBottom->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -15.f), FRotator(90.f, 0.f, -45.f));
	SceneCaptureBottom->FOVAngle = 20.f;
	SceneCaptureBottom->bCaptureEveryFrame = false;
	SceneCaptureBottom->bCaptureOnMovement = false;
}


// Called when the game starts
void ULightGemComponent::BeginPlay()
{
	Super::BeginPlay();

	RenderTargetTop = NewObject<UTextureRenderTarget2D>();
	RenderTargetTop->InitCustomFormat(LIGHTGEM_TEX_WIDTH, LIGHTGEM_TEX_HEIGHT, EPixelFormat::PF_FloatRGB, false);
	SceneCaptureTop->TextureTarget = RenderTargetTop;


	RenderTargetBottom = NewObject<UTextureRenderTarget2D>();
	RenderTargetBottom->InitCustomFormat(LIGHTGEM_TEX_WIDTH, LIGHTGEM_TEX_HEIGHT, EPixelFormat::PF_FloatRGB, false);
	SceneCaptureBottom->TextureTarget = RenderTargetBottom;
}


// Called every frame
void ULightGemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ComputeBrightness();
}


void ULightGemComponent::ComputeBrightness()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, LightUpdateInterval, FColor::Yellow, FString::Printf(TEXT("%f"), BrightnessOutput));

	if (!RenderTargetTop || !RenderTargetBottom)
	{
		BrightnessOutput = 0.f;
		return;
	}

	if (CaptureFenceTop.IsFenceComplete() && CaptureFenceBottom.IsFenceComplete())
	{
		float BrightnessTop = FindBrightestPixel(RenderTargetTop);
		float BrightnessBottom = FindBrightestPixel(RenderTargetBottom);

		BrightnessOutput = FMath::Max(BrightnessTop, BrightnessBottom) * LIGHTGEM_SCALE;

		SceneCaptureTop->CaptureSceneDeferred();
		CaptureFenceTop.BeginFence();

		SceneCaptureBottom->CaptureSceneDeferred();
		CaptureFenceBottom.BeginFence();
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, LightUpdateInterval, FColor::Emerald, FString::Printf(TEXT("all scene captures is not ready yet")));
	}
}


float ULightGemComponent::FindBrightestPixel(UTextureRenderTarget2D* TextureTarget)
{
	float CurrentBrightestPixel = 0.f;

	TArray<FColor> TexturePixels;

	FRenderTarget* RenderTarget = TextureTarget->GameThread_GetRenderTargetResource();
	RenderTarget->ReadPixels(TexturePixels);

	for (const auto& Pixel : TexturePixels)
	{
		// Use a formula to determine brightness based on pixel color values
		float PixelBrightness = Pixel.R * LIGHTGEM_R + Pixel.G * LIGHTGEM_G + Pixel.B * LIGHTGEM_B;

		if (PixelBrightness > CurrentBrightestPixel)
		{
			CurrentBrightestPixel = PixelBrightness;
		}
	}

	return CurrentBrightestPixel;
}
