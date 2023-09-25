#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "XR_Project_Team10/CommonMonster/ICommonMonsterBase.h"
#include "XR_Project_Team10/CommonMonster/CommonMonsterDataAsset.h"
#include "UObject/ConstructorHelpers.h"
#include "CommonMonster.generated.h"

UENUM()
enum EState : uint8 {
	E_IDLE,
	E_RESEARCH,
	E_RECOGNITION,
	E_TRACKING,
	E_ATTACK_CONFIG,
	E_ATTACK_OMEN,
	E_ATTACK,
	E_HIT,
	E_DEAD
};

UCLASS()
class XR_PROJECT_TEAM10_API ACommonMonster : public ACharacter, public IICommonMonsterBase
{
	GENERATED_BODY()

public:
	ACommonMonster();

	// IICommonMonsterBase을(를) 통해 상속
protected:
	virtual void Research() override;
	virtual void Recognition() override;
	virtual void Tracking() override;
	virtual void AttackConfig() override;
	virtual void AttackOmen() override;
	virtual void Attack() override;
	virtual void Dead() override;

protected:
	//DataAsset MonsterData => IICommonMonsterBase

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> MonsterStaticMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCapsuleComponent> MonsterComponent;

	//변수
protected:
	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<EState> MonsterState;

	UPROPERTY(VisibleAnywhere)
	uint32 MonsterCurrentHP;

	UPROPERTY(VisibleAnywhere)
	float MonsterCurrentMoveSpeed;
	
	//Getter Setter
public:
	uint32 GetMonsterHP();
	void SetMonsterHP(uint32);

	float GetMonsterSpeed();
	void SetMonsterSpeed(float);

	TEnumAsByte<EState> GetMonsterState();

};
