/*
@Copyright Based Development.
2022, 2023 Unpublished Work.
*/


#include "LandingCameraShake.h"


ULandingCameraShake::ULandingCameraShake()
{
	OscillationDuration = 0.2f;
	OscillationBlendInTime = 0.f;
	OscillationBlendOutTime = 0.2f;

	LocOscillation.Z.Amplitude = -20.f;
	LocOscillation.Z.Frequency = 10.f;
	LocOscillation.Z.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
	LocOscillation.Z.Waveform = EOscillatorWaveform::SineWave;
}
