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

	// ���� �Լ�
protected:
	virtual void Research() = 0;		//Ž��
	virtual void Recognition() = 0;		//����
	virtual void Tracking() = 0;		//����
	virtual void AttackConfig() = 0;	//���� Ȯ��
	virtual void AttackOmen() = 0;		//���� ����
	virtual void Attack() = 0;			//����
	virtual void Dead() = 0;			//���

	// ���� ����
protected:
	TObjectPtr<class CommonMonsterDataAsset> MonsterData;
};
