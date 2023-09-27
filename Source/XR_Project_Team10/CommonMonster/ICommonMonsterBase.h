#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "XR_Project_Team10/CommonMonster/CommonMonsterDataAsset.h"
#include "ICommonMonsterBase.generated.h"

UINTERFACE(MinimalAPI)
class UICommonMonsterBase : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class XR_PROJECT_TEAM10_API IICommonMonsterBase
{
	GENERATED_BODY()

	// 공통 함수
protected:
	virtual void Research() = 0;		//탐색
	virtual void Recognition() = 0;		//인지
	virtual void Tracking() = 0;		//추적
	virtual void AttackConfig() = 0;	//공격 확인
	virtual void AttackOmen() = 0;		//공격 전조
	virtual void Attack() = 0;			//공격
	virtual void Dead() = 0;			//사망

	// 공통 변수
protected:
	TObjectPtr<class UCommonMonsterDataAsset> MonsterData;
};
