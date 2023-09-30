// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "XR_Project_Team10/Interface/KWBossMonsterInterface.h"
#include "KWBossMonsterBase.generated.h"

UCLASS()
class XR_PROJECT_TEAM10_API AKWBossMonsterBase : public ACharacter//, public IKWBossMonsterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKWBossMonsterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
