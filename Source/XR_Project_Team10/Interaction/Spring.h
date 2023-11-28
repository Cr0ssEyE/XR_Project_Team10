// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "Spring.generated.h"

UCLASS()
class XR_PROJECT_TEAM10_API ASpring : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpring();

protected:
	virtual void BeginPlay() override;

public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	FVector CurvePoints(float Control);
	void MoveToPoint();

public:
	UPROPERTY(EditAnywhere, DisplayName = "도착 위치")
	FVector EndPosition;
	
	UPROPERTY(EditAnywhere, DisplayName = "곡선 높이")
	float ZValue = 100;

	UPROPERTY(EditAnywhere, DisplayName = "소요 시간 (초)")
	float Time = 5;

	UPROPERTY(EditAnywhere, DisplayName = "매 틱 (초)")
	float TimerTick = 0.01f;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY()
	TObjectPtr<AKWPlayerCharacter> Player;

	UPROPERTY()
	FVector StartTopPos = FVector::ZeroVector;

	UPROPERTY()
	FVector EndTopPos = FVector::ZeroVector;

	UPROPERTY()
	FVector StartPosition;

	UPROPERTY()
	float ControlPoint = 0;

	FTimerHandle MoveToTimerHandle;
};
