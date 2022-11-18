/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "LightGem.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/KismetRenderingLibrary.h"


// Sets default values
ALightGem::ALightGem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Init textures
	//OctahedronTopTexture = NewObject<UTextureRenderTarget2D>();
	//OctahedronTopTexture->InitAutoFormat(128, 128);

	//OctahedronBottomTexture = NewObject<UTextureRenderTarget2D>();
	//OctahedronBottomTexture->InitAutoFormat(128, 128);

	// Init octahedron mesh
	Octahedron = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Octahedron"));
	Octahedron->SetupAttachment(RootComponent);
	Octahedron->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> OctahedronAsset(
		TEXT("StaticMesh'/Game/Vincent/LightLevel/Octahedron.Octahedron'"));

	if (OctahedronAsset.Succeeded())
	{
		Octahedron->SetStaticMesh(OctahedronAsset.Object);
	}

	// Init octahedron captures
	OctahedronTopCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Octahedron Top Capture"));
	OctahedronTopCapture->SetupAttachment(Octahedron);
	OctahedronTopCapture->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 15.f), FRotator(-90.f, 0.f, 45.f));
	OctahedronTopCapture->bAutoActivate = false;
	OctahedronTopCapture->bCaptureOnMovement = false;
	OctahedronTopCapture->bCaptureEveryFrame = false;

	OctahedronBottomCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Octahedron Bottom Capture"));
	OctahedronBottomCapture->SetupAttachment(Octahedron);
	OctahedronBottomCapture->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -15.f), FRotator(90.f, 0.f, -45.f));
	OctahedronBottomCapture->bAutoActivate = false;
	OctahedronBottomCapture->bCaptureOnMovement = false;
	OctahedronBottomCapture->bCaptureEveryFrame = false;
}

// Called when the game starts or when spawned
void ALightGem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALightGem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleLightLevel();
}

void ALightGem::HandleLightLevel()
{
	UTextureRenderTarget2D* TopTexture = NewObject<UTextureRenderTarget2D>();
	TopTexture->InitAutoFormat(128, 128);

	UTextureRenderTarget2D* BottomTexture = NewObject<UTextureRenderTarget2D>();
	BottomTexture->InitAutoFormat(128, 128);


	if (TopTexture == nullptr || BottomTexture == nullptr)
	{
		LightLevel = 0.f;
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("Texture are missed!")));
		return;
	}

	OctahedronTopCapture->TextureTarget = TopTexture;
	OctahedronTopCapture->CaptureScene();

	OctahedronBottomCapture->TextureTarget = BottomTexture;
	OctahedronBottomCapture->CaptureScene();

	float MaxBrightnessTop = GetTextureMaxPixelBrightness(TopTexture);
	float MaxBrightnessBottom = GetTextureMaxPixelBrightness(BottomTexture);

	LightLevel = FMath::Max(MaxBrightnessBottom, MaxBrightnessTop);

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("%f"), LightLevel));
}

float ALightGem::GetTextureMaxPixelBrightness(UTextureRenderTarget2D* TextureTarget)
{
	float CurrentBrightestPixel = 0.f;

	TArray<FColor> TexturePixels;

	FRenderTarget* RenderTarget = TextureTarget->GameThread_GetRenderTargetResource();
	RenderTarget->ReadPixels(TexturePixels);

	for (const auto &Pixel : TexturePixels)
	{
		// Use a formula to determine brightness based on pixel color values. Source for Formula used:
		float PixelBrightness = 0.299 * Pixel.R + 0.587 * Pixel.G + 0.114 * Pixel.B;

		if (PixelBrightness > CurrentBrightestPixel)
		{
			CurrentBrightestPixel = PixelBrightness;
		}
	}

	return CurrentBrightestPixel;
}
