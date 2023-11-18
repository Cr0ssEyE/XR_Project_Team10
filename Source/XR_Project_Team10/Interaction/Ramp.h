// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Ramp.generated.h"

UCLASS()
class XR_PROJECT_TEAM10_API ARamp : public AActor
{
	GENERATED_BODY()
	
public:	
	ARamp();

protected:
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

private:
	UPROPERTY(EditAnywhere, DisplayName = "감속 값 (0~1)")
	float DecelerationValue;

	UPROPERTY(EditAnywhere, DisplayName = "추가될 Y Value값 (배수)")
	float JumpValue;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY()
	float StartZ = 0;

	UPROPERTY()
	float CheckZ = 50;	// Half of Collision.Z
};
