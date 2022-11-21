/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "LightGem.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/KismetRenderingLibrary.h"

// Light gem texture render sizes
static constexpr uint32 LIGHTGEM_TEX_WIDTH = 64;
static constexpr uint32 LIGHTGEM_TEX_HEIGHT = 64;

// The brightness of octahedron's pixel is from 0 to 255.
// We multiplying current brightness and LIGHTGEM_SCALE for normalize the output value (from 0 to 1)
static constexpr float LIGHTGEM_SCALE = 1.f / 255.f;

// Coefficients by which to multiply the corresponding color values for the brightness formula
static constexpr float LIGHTGEM_R = 0.299;
static constexpr float LIGHTGEM_G = 0.587;
static constexpr float LIGHTGEM_B = 0.114;

// Sets default values
ALightGem::ALightGem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Init octahedron mesh
	Octahedron = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Octahedron"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> OctahedronAsset(
		TEXT("StaticMesh'/Game/Vincent/LightLevel/Octahedron.Octahedron'"));

	if (OctahedronAsset.Succeeded())
	{
		Octahedron->SetStaticMesh(OctahedronAsset.Object);
	}

	Octahedron->SetupAttachment(RootComponent);
	Octahedron->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	Octahedron->SetCastShadow(false);
	// Lightgem system is a backend process, so player should not see the octahedron
	Octahedron->SetVisibleInSceneCaptureOnly(true);

	// Init octahedron captures
	OctahedronTopCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Octahedron Top Capture"));
	OctahedronTopCapture->SetupAttachment(Octahedron);
	OctahedronTopCapture->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 15.f), FRotator(-90.f, 0.f, 45.f));
	OctahedronTopCapture->FOVAngle = 20.f;

	OctahedronBottomCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Octahedron Bottom Capture"));
	OctahedronBottomCapture->SetupAttachment(Octahedron);
	OctahedronBottomCapture->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -15.f), FRotator(90.f, 0.f, -45.f));
	OctahedronBottomCapture->FOVAngle = 20.f;
}

// Called when the game starts or when spawned
void ALightGem::BeginPlay()
{
	Super::BeginPlay();

	TopTexture = NewObject<UTextureRenderTarget2D>();
	TopTexture->InitCustomFormat(LIGHTGEM_TEX_WIDTH, LIGHTGEM_TEX_HEIGHT, EPixelFormat::PF_FloatRGB, false);
	OctahedronTopCapture->TextureTarget = TopTexture;

	BottomTexture = NewObject<UTextureRenderTarget2D>();
	BottomTexture->InitCustomFormat(LIGHTGEM_TEX_WIDTH, LIGHTGEM_TEX_HEIGHT, EPixelFormat::PF_FloatRGB, false);
	OctahedronBottomCapture->TextureTarget = BottomTexture;
}

// Called every frame
void ALightGem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleLightLevel();
}

void ALightGem::HandleLightLevel()
{
	if (!TopTexture|| !BottomTexture)
	{
		LightLevel = 0.f;
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("Texture are missed!")));
		return;
	}

	float MaxBrightnessTop = AnalyzeTexture(TopTexture);
	float MaxBrightnessBottom = AnalyzeTexture(BottomTexture);

	LightLevel = FMath::Max(MaxBrightnessBottom, MaxBrightnessTop) * LIGHTGEM_SCALE;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("%f"), LightLevel));
}

float ALightGem::AnalyzeTexture(UTextureRenderTarget2D* TextureTarget)
{
	float CurrentBrightestPixel = 0.f;

	TArray<FColor> TexturePixels;

	FRenderTarget* RenderTarget = TextureTarget->GameThread_GetRenderTargetResource();
	RenderTarget->ReadPixels(TexturePixels);

	for (const auto &Pixel : TexturePixels)
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