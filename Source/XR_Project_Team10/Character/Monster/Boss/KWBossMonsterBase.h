// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "XR_Project_Team10/Interface/KWBossMonsterInterface.h"
#include "KWBossMonsterBase.generated.h"

UCLASS()
class XR_PROJECT_TEAM10_API AKWBossMonsterBase : public ACharacter, public IKWBossMonsterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKWBossMonsterBase();

	FORCEINLINE virtual UDataAsset* GetBossStatusData() override { return BossMonsterStatusData; }

	FORCEINLINE virtual UDataAsset* GetBossAnimData() override { return BossMonsterAnimData; }
	
	FORCEINLINE virtual UDataAsset* GetBossAIData() override { return BossMonsterAIData; }
	
	FORCEINLINE virtual void SetState(const EMonsterState State) override { CurrentState = State; }

	virtual void SetAIPatternDelegate(const FAICharacterPatternFinished& PatternFinishedDelegate) override { }
	virtual void EndEncounterAnimation() override { }
	virtual void PlayPatternAnimMontage() override { }
	virtual void PlayDeadAnimation() override { }
	virtual void DeActivateInGame() override { }
	virtual void ReActivateInGame() override { }
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void InitData() override { }
	
protected:
	UPROPERTY()
	TObjectPtr<class UDataAsset> BossMonsterStatusData;

	UPROPERTY()
	TObjectPtr<class UDataAsset> BossMonsterAnimData;

	UPROPERTY()
	TObjectPtr<class UDataAsset> BossMonsterAIData;

	UPROPERTY()
	TObjectPtr<class UAnimMontage> BossAnimMontage;
	
protected:
	float BossHp;

	float MoveSpeed;
	
	EMonsterState CurrentState;

	uint8 bIsMoving = 0;
	
	uint8 bIsAttacking = 0;

	uint8 bIsPatternRunning = 0;
};
