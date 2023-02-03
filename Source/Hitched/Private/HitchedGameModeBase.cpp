/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "HitchedGameModeBase.h"
#include "Vincent/VincentBloodberry.h"
#include "Vincent/VincentHUD.h"


AHitchedGameModeBase::AHitchedGameModeBase()
{
	DefaultPawnClass = AVincentBloodberry::StaticClass();

	HUDClass = AVincentHUD::StaticClass();
}