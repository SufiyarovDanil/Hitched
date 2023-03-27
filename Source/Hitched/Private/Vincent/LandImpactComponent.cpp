/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "Vincent/LandImpactComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveVector.h"


// Sets default values for this component's properties
ULandImpactComponent::ULandImpactComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Land Impact Init
	LandImpactTL = CreateDefaultSubobject<UTimelineComponent>(TEXT("Land Timeline"));

	static ConstructorHelpers::FObjectFinder<UCurveVector> LandCurveAsset(
		TEXT("CurveVector'/Game/Vincent/TimelineAnims/LandingCameraImpactCurve.LandingCameraImpactCurve'"));

	if (LandCurveAsset.Succeeded())
	{
		LandCurve = LandCurveAsset.Object;

		FOnTimelineVector TimelineCallback;

		TimelineCallback.BindUFunction(this, FName(TEXT("LandTLProgress")));

		LandImpactTL->AddInterpVector(LandCurve, TimelineCallback);
		LandImpactTL->SetLooping(false);
		LandImpactTL->SetTimelineLength(LandCurve->FloatCurves[2].GetLastKey().Time);
	}
}


// Called when the game starts
void ULandImpactComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void ULandImpactComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void ULandImpactComponent::StartAnimation()
{
	if (!LandImpactTL)
	{
		return;
	}

	LandImpactTL->PlayFromStart();
}


void ULandImpactComponent::LandTLProgress(const FVector Location)
{
	SetRelativeLocation(Location);
}
