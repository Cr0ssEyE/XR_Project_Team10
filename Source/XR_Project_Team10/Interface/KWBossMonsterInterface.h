// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "XR_Project_Team10/Enumeration/KWMonsterState.h"
#include "KWBossMonsterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UKWBossMonsterInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE(FAICharacterPatternFinished)

/**
 * 보스 몬스터 전반에 필수적으로 들어가야 할 함수 인터페이스
 **/
class XR_PROJECT_TEAM10_API IKWBossMonsterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void InitData() = 0;
	virtual UDataAsset* GetBossStatusData() = 0;
	virtual UDataAsset* GetBossAnimData() = 0;
	virtual UDataAsset* GetBossAIData() = 0;
	virtual void SetState(EMonsterState State) = 0;

	virtual void SetAIPatternDelegate(const FAICharacterPatternFinished& PatternFinishedDelegate) = 0;
	virtual void EndEncounterAnimation() = 0;
	virtual void PlayPatternAnimMontage() = 0;
	virtual void PlayDeadAnimation() = 0;
	virtual void DeActivateInGame() = 0;
	virtual void ReActivateInGame() = 0;
};
