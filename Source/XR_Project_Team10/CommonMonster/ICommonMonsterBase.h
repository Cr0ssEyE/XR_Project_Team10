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
//DECLARE_DELEGATE_OneParam(FCommonAttackFinished, FVector);
DECLARE_DELEGATE(FCommonAttackFinished);
/**
 * 
 */
class XR_PROJECT_TEAM10_API IICommonMonsterBase
{
	GENERATED_BODY()

	public:
	virtual void SetCommonAttackDelegate(const FCommonAttackFinished& InOnAttackFinished) = 0;
	virtual void CommonMonsterAttack(AActor* Target) = 0;
	virtual void CommonMonsterDead() = 0;			//사망

protected:
	virtual void AttackOmen() = 0;		//공격 전조
	virtual void Attack() = 0;			//공격


protected:
	// AI에 필요한 변수
	FCommonAttackFinished OnAttackFinished;

	FVector MonsterBaseLocation;
	float MonsterMaximumMoveDistance;
	float MonsterTurnSpeed;

public:
	TObjectPtr<class UCommonMonsterDataAsset> MonsterData;
};
