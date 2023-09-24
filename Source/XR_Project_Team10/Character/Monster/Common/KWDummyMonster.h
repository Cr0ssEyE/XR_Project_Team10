// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "XR_Project_Team10/Interface/KWMonsterAIInterface.h"
#include "KWDummyMonster.generated.h"

UCLASS()
class XR_PROJECT_TEAM10_API AKWDummyMonster : public ACharacter, public IKWMonsterAIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKWDummyMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual FVector GetAIBaseLocation() override { return BaseLocation; }
	virtual float GetAIMaximumMoveDistance() override { return MaximumMoveDistance; }
	virtual float GetAIPatrolRadius() override { return 500.f; }
	virtual float GetAIDetectRange() override { return 300.f; }
	virtual float GetAIAttackRange() override { return AttackRange; }
	virtual float GetAITurnSpeed() override { return TurnSpeed; }

private:
	FVector BaseLocation;
	float MaximumMoveDistance;
	float PatrolRadius;
	float DetectRange;
	float AttackRange;
	float TurnSpeed;
};
