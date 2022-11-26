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
 * �������� LightGem �������� �� ��������� ���������:
 * � �������� ����������� ������ ������������ (��� �� ������� ���������)
 * ������������ �������, �� ������� ������ ��������� ��������� �� ������.
 * �������, � ����������� �� ��������� �����, ���������� ������� ���
 * ������ � ��� Scene capture ���������� ������ *������* �������� ������
 * � �����. ����� ����� �������� �� ���� ������������� ������� � �������
 * ������ ������ �������, ��������� �������� � ���������� � ����
 * LightLevel. ��� ��� �������� ���������� ������ ����.
 */
UCLASS()
class HITCHED_API ALightGem : public AActor
{
	GENERATED_BODY()
	
public:	

	/* ���������� �������� ������ ������������ */
	float GetLightScale() const { return LightLevel; }

	/* ���������� ��� ��������� �������� ����, ����� ��������� ����� ������ */
	UFUNCTION(BlueprintCallable)
	UTextureRenderTarget2D* GetTopTextureRenderTarget() const { return TopTexture; }

protected:

	/* �������, �� ������� �������� ��� ������� ���� */
	UPROPERTY(VisibleAnywhere, Category = "Octahedron Static Mesh")
	UStaticMeshComponent* Octahedron = nullptr;

	/* ������ �����, ������� �������� ������� ����� �������� */
	UPROPERTY(VisibleAnywhere, Category = "Octahedron Snapshot")
	USceneCaptureComponent2D* OctahedronTopCapture = nullptr;

	/* ������ �����, ������� �������� ������ ����� �������� */
	UPROPERTY(VisibleAnywhere, Category = "Octahedron Snapshot")
	USceneCaptureComponent2D* OctahedronBottomCapture = nullptr;

private:

#pragma region FUNCTIONS

	/* ����������� */
	ALightGem();

	/* ����������, ����� ���� Actor ��������� �� ������ */
	virtual void BeginPlay() override;

	/* ���������� ������ ���� */
	virtual void Tick(float DeltaTime) override;

	/*
	* �����, � ������� ����������� ������� ������������ �����
	* ������� ������������� ������ � ������� ������ �������� �
	* ���������� ���� LightLevel ������������ �������������
	* �������� (�� 0.f �� 1.f)
	*/
	void HandleLightLevel();

	/* 
	* ������� ����� ����� ������� � ������������� �����������
	* � ���������� ��� �������� (�� 0 �� 255) 
	*/
	float AnalyzeTexture(UTextureRenderTarget2D* TextureTarget);

#pragma endregion

#pragma region VARIABLES

	/*
	* ������ � ���� ������� ������������.
	* ��������� �������� �� 0.f �� 1.f.
	*/
	UPROPERTY(VisibleAnywhere, Category = "Light Level")
	float LightLevel = 0.f;

	/*
	* Texture target ��� ������� �����, ����������� �� ������
	* ������� ����� ��������
	*/
	UPROPERTY(VisibleAnywhere, Category = "Render Target")
	UTextureRenderTarget2D* TopTexture = nullptr;

	/*
	* Texture target ��� ������� �����, ����������� �� ������
	* ������ ����� ��������
	*/
	UPROPERTY(VisibleAnywhere, Category = "Render Target")
	UTextureRenderTarget2D* BottomTexture = nullptr;

#pragma endregion
};