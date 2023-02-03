/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "Vincent/VincentHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "Blueprint/UserWidget.h"


AVincentHUD::AVincentHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> OctahedronWidgetAsset(
		TEXT("/Game/Vincent/UIInterface"));

	if (OctahedronWidgetAsset.Succeeded())
	{
		LightGemWidgetClass = OctahedronWidgetAsset.Class;
	}
}

void AVincentHUD::BeginPlay()
{
	Super::BeginPlay();

	if (LightGemWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), LightGemWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void AVincentHUD::DrawHUD()
{
	Super::DrawHUD();

	if (LightGemTex)
	{
		// find center of the Canvas
		const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

		// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
		const FVector2D CrosshairDrawPosition(Center.X, Center.Y + 20.0f);

		// draw the crosshair
		FCanvasTileItem TileItem(CrosshairDrawPosition, LightGemTex->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}

void AVincentHUD::UpdateTexture(UTexture2D* NewTexture)
{
	LightGemTex = NewTexture;
}