#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
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

private:
	IICommonMonsterBase();

	// 공통 함수
protected:
	virtual void Research() = 0;		//탐색
	virtual void Recognition() = 0;		//인지
	virtual void Tracking() = 0;		//추적
	virtual void AttackConfig() = 0;	//공격 확인
	virtual void AttackOmen() = 0;		//공격 전조
	virtual void Attack() = 0;			//공격
	virtual void Hit() = 0;				//피격
	virtual void Dead() = 0;			//사망

	// 공통 변수
	// 상속받은 후, return 으로 각 값을 넘겨주는 식으로 초기화합니다
protected:
	//UPROPERTY(VisibleAnywshere)
	TObjectPtr<class CommonMonsterDataAsset> MonsterData;
};
