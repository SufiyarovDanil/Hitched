/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightGem.generated.h"


class USceneCaptureComponent2D;

/*
 * Механика LightGem работает по следующим принципам:
 * В качестве определения уровня освещенности (или же уровень видимости)
 * используется октаэдр, на который светят источники освещения на уровне.
 * Октаэдр, в зависимости от падающего света, становится светлее или
 * темнее и два Scene capture компонента делают *снимок* октаэдра сверху
 * и снизу. После этого проходим по двум отрендеренным снимкам в поисках
 * самого яркого пикселя, нормирует значение и записываем в поле
 * LightLevel. Все эти действия происходят каждый кадр.
 */
UCLASS()
class HITCHED_API ALightGem : public AActor
{
	GENERATED_BODY()
	
public:	

	/* Возвращает значение уровня освещенности */
	float GetLightScale() const { return LightLevel; }

	/* Необходимо для настройки захватов сцен, после настройки можно убрать */
	UFUNCTION(BlueprintCallable)
	UTextureRenderTarget2D* GetTopTextureRenderTarget() const { return TopTexture; }

protected:

	/* Октаэдр, на который наведены два захвата сцен */
	UPROPERTY(VisibleAnywhere, Category = "Octahedron Static Mesh")
	UStaticMeshComponent* Octahedron = nullptr;

	/* Захват сцены, который рендерит верхнюю часть октаэдра */
	UPROPERTY(VisibleAnywhere, Category = "Octahedron Snapshot")
	USceneCaptureComponent2D* OctahedronTopCapture = nullptr;

	/* Захват сцены, который рендерит нижнюю часть октаэдра */
	UPROPERTY(VisibleAnywhere, Category = "Octahedron Snapshot")
	USceneCaptureComponent2D* OctahedronBottomCapture = nullptr;

private:

#pragma region FUNCTIONS

	/* Конструктор */
	ALightGem();

	/* Вызывается, когда этот Actor спавнится на уровне */
	virtual void BeginPlay() override;

	/* Вызывается каждый кадр */
	virtual void Tick(float DeltaTime) override;

	/*
	* Метод, в котором вычисляется уровень освещенности путем
	* анализа отрендеренных нижней и верхней частей октаэдра и
	* присвоения полю LightLevel максимальное нормированное
	* значение (от 0.f до 1.f)
	*/
	void HandleLightLevel();

	/* 
	* Находит самый яркий пиксель в отрендеренном изображении
	* и возвращает его значение (от 0 до 255) 
	*/
	float AnalyzeTexture(UTextureRenderTarget2D* TextureTarget);

#pragma endregion

#pragma region VARIABLES

	/*
	* Хранит в себе уровень освещенности.
	* Принимает значение от 0.f до 1.f.
	*/
	UPROPERTY(VisibleAnywhere, Category = "Light Level")
	float LightLevel = 0.f;

	/*
	* Texture target для захвата сцены, отвечающего за рендер
	* верхней части октаэдра
	*/
	UPROPERTY(VisibleAnywhere, Category = "Render Target")
	UTextureRenderTarget2D* TopTexture = nullptr;

	/*
	* Texture target для захвата сцены, отвечающего за рендер
	* нижней части октаэдра
	*/
	UPROPERTY(VisibleAnywhere, Category = "Render Target")
	UTextureRenderTarget2D* BottomTexture = nullptr;

#pragma endregion
};