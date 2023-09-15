#pragma once

#include "CoreMinimal.h"
#include "Components/LineBatchComponent.h"

class XR_PROJECT_TEAM10_API FPPDrawLineHelper
{
public:
	static void DrawLine(const UWorld* InWorld, FVector const& LineStart, FVector const& LineEnd, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
	{
		bool bDepthIsForeground = (0 == SDPG_Foreground);

		ULineBatchComponent* LineBatcher = (InWorld ? (bDepthIsForeground ? InWorld->ForegroundLineBatcher : ((bPersistentLines || (LifeTime > 0.f)) ? InWorld->PersistentLineBatcher : InWorld->LineBatcher)) : nullptr);

		if (LineBatcher)
		{
			LineBatcher->DrawLine(LineStart, LineEnd, Color, 0, Thickness, LifeTime);
		}
	}
	
	static void DrawSphere(const UWorld* InWorld, FVector const& Center, float Radius, int32 Segments, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
	{
		// this means foreground lines can't be persistent 
		if (ULineBatchComponent* const LineBatcher = GetLineBatcher(InWorld, bPersistentLines, LifeTime, (DepthPriority == SDPG_Foreground)))
		{
			float LineLifeTime = GetLineLifeTime(LineBatcher, LifeTime, bPersistentLines);

			// Need at least 4 segments
			Segments = FMath::Max(Segments, 4);

			FVector Vertex1, Vertex2, Vertex3, Vertex4;
			const float AngleInc = 2.f * UE_PI / float(Segments);
			int32 NumSegmentsY = Segments;
			float Latitude = AngleInc;
			int32 NumSegmentsX;
			float Longitude;
			float SinY1 = 0.0f, CosY1 = 1.0f, SinY2, CosY2;
			float SinX, CosX;

			TArray<FBatchedLine> Lines;
			Lines.Empty(NumSegmentsY * Segments * 2);
			while (NumSegmentsY--)
			{
				SinY2 = FMath::Sin(Latitude);
				CosY2 = FMath::Cos(Latitude);

				Vertex1 = FVector(SinY1, 0.0f, CosY1) * Radius + Center;
				Vertex3 = FVector(SinY2, 0.0f, CosY2) * Radius + Center;
				Longitude = AngleInc;

				NumSegmentsX = Segments;
				while (NumSegmentsX--)
				{
					SinX = FMath::Sin(Longitude);
					CosX = FMath::Cos(Longitude);

					Vertex2 = FVector((CosX * SinY1), (SinX * SinY1), CosY1) * Radius + Center;
					Vertex4 = FVector((CosX * SinY2), (SinX * SinY2), CosY2) * Radius + Center;

					Lines.Add(FBatchedLine(Vertex1, Vertex2, Color, LineLifeTime, Thickness, DepthPriority));
					Lines.Add(FBatchedLine(Vertex1, Vertex3, Color, LineLifeTime, Thickness, DepthPriority));

					Vertex1 = Vertex2;
					Vertex3 = Vertex4;
					Longitude += AngleInc;
				}
				SinY1 = SinY2;
				CosY1 = CosY2;
				Latitude += AngleInc;
			}
			LineBatcher->DrawLines(Lines);
		}

	}
	
	static ULineBatchComponent* GetLineBatcher(const UWorld* InWorld, bool bPersistentLines, float LifeTime, bool bDepthIsForeground)
	{
		return (InWorld ? (bDepthIsForeground ? InWorld->ForegroundLineBatcher : ((bPersistentLines || (LifeTime > 0.f)) ? InWorld->PersistentLineBatcher : InWorld->LineBatcher)) : nullptr);
	}

	static float GetLineLifeTime(ULineBatchComponent* LineBatcher, float LifeTime, bool bPersistent)
	{
		return bPersistent ? -1.0f : ((LifeTime > 0.f) ? LifeTime : LineBatcher->DefaultLifeTime);
	}
};
