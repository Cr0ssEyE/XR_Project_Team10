#pragma once

#include <chrono>

#include "CoreMinimal.h"

class XR_PROJECT_TEAM10_API FPPTimerHelper
{
private:
	typedef std::chrono::high_resolution_clock FClock;
	typedef std::chrono::high_resolution_clock::time_point FTimePoint;
	typedef std::chrono::duration<double> FDuration;

	static TMap<FTimerHandle, FTimePoint> DeltaTimeMap;
	static TMap<FTimerHandle, FTimePoint> DelayMap;

public:
	static float GetActualDeltaTime(const FTimerHandle& InHandle)
	{
		const FTimePoint Time = FClock::now();;

		if (!DeltaTimeMap.Contains(InHandle))
		{
			DeltaTimeMap.Emplace(InHandle, Time);
		}

		const FDuration Result = Time - DeltaTimeMap[InHandle];
		DeltaTimeMap[InHandle] = Time;

		return Result.count();
	}

	static bool IsDelayElapsed(const FTimerHandle& InHandle, const double InDelay)
	{
		const FTimePoint Time = FClock::now();;

		if (!DelayMap.Contains(InHandle))
		{
			DelayMap.Emplace(InHandle, Time);
		}

		const FDuration ElapsedTime = Time - DelayMap[InHandle];
		return ElapsedTime.count() >= InDelay;
	}

	static void InvalidateTimerHandle(const FTimerHandle& InHandle)
	{
		DeltaTimeMap.Remove(InHandle);
	}
};
