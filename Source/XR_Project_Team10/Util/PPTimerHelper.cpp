#include "PPTimerHelper.h"

TMap<FTimerHandle, FPPTimerHelper::FTimePoint> FPPTimerHelper::DeltaTimeMap;
TMap<FTimerHandle, FPPTimerHelper::FTimePoint> FPPTimerHelper::DelayMap;