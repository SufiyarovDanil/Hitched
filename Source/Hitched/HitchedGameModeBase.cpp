/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "HitchedGameModeBase.h"
#include "VincentBloodberry.h"
#include "VincentHUD.h"


AHitchedGameModeBase::AHitchedGameModeBase()
{
	DefaultPawnClass = AVincentBloodberry::StaticClass();

	HUDClass = AVincentHUD::StaticClass();
}