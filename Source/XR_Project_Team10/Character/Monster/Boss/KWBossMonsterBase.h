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
	
	virtual void SetState(EMonsterState State) override { }
	virtual void PlayEncounterAnimation() override { }
	virtual void PlayDeadAnimation() override { }
	virtual void DeActivateInGame() override { }
	virtual void ReActivateInGame() override { }
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	virtual void InitData() override { }
	
private:
	UPROPERTY()
	TObjectPtr<class UDataAsset> BossMonsterStatusData;

	UPROPERTY()
	TObjectPtr<class UDataAsset> BossMonsterAnimData;

	UPROPERTY()
	TObjectPtr<class UDataAsset> BossMonsterAIData;
	
	UPROPERTY()
	TObjectPtr<class UAnimMontage> BossEncounterMontage;
	
	UPROPERTY()
	TObjectPtr<class UAnimMontage> BossPatternMontage;

	UPROPERTY()
	TObjectPtr<class UAnimMontage> BossDeadMontage;
	
private:
	float BossHp;

	EMonsterState CurrentState;

	uint8 bIsMoving = 0;
	
	uint8 bIsAttacking = 0;
};
