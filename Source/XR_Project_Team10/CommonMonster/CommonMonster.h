#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "XR_Project_Team10/CommonMonster/ICommonMonsterBase.h"
#include "XR_Project_Team10/CommonMonster/CommonMonsterDataAsset.h"
#include "XR_Project_Team10/Util/PPTimerHelper.h"
#include "UObject/ConstructorHelpers.h"
#include "CommonMonster.generated.h"

UENUM()
enum class EState : uint8 {
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

/*
* 해당 클래스의 자식들과 관련된 클래스의 경우, 해당 객체의 이름을 약어로 사용합니다
* (이는 해당 클래스가 어디에 쓰이는지 아는 것에 용이하게 설계하기 위해서 입니다)
* 
* DarkWing - DW
* 
*/


UCLASS()
class XR_PROJECT_TEAM10_API ACommonMonster : public ACharacter, public IICommonMonsterBase
{
	GENERATED_BODY()

public:
	ACommonMonster();

	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// IICommonMonsterBase을(를) 통해 상속
protected:
	virtual void BeginPlay() override;

	virtual void SetCommonAttackDelegate(const FCommonAttackFinished& InOnAttackFinished) override;

	virtual void AttackOmen() override;
	virtual void Attack() override;
	void CheckAttackDelay();

	virtual void CommonMonsterAttack(AActor* Target) override;
	virtual void CommonMonsterDead() override;
	void PlayDeadAnimation();
	void AfterDead();

	virtual void ApplyKnockBack();


protected:
	//DataAsset MonsterData => IICommonMonsterBase

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPrimitiveComponent> MonsterComponent;

	//변수
protected:
	UPROPERTY(VisibleAnywhere)
	EState MonsterState;

	UPROPERTY(VisibleAnywhere)
	uint32 MonsterCurrentHP;

	UPROPERTY(VisibleAnywhere)
	float MonsterCurrentMoveSpeed;

	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "공격 쿨타임")
	float MonsterAttackCoolDownTime;

	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "공격 전조 진행시간")
	float MonsterAttackOmenTime;

	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "공격 진행시간")
	float MonsterAttackTime;

	UPROPERTY()
	float KnockBackElapsedTime;

	UPROPERTY()
	FVector KnockBackImpactLocation;

	UPROPERTY(EditAnywhere, Category = KnockBack, DisplayName = "넉백량")
	float KnockBackAmount = 10.0f;

	UPROPERTY(EditAnywhere, Category = Dead, DisplayName = "사망 후 비활성화되는 시간")
	float MonsterDisableTime = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY()
	AActor* PlayerTarget;

	FTimerHandle AttackCoolDownTimerHandle;
	FTimerHandle AttackOmenTimerHandle;
	FTimerHandle AttackTimerHandle;
	FTimerHandle KnockBackTimerHandle;
	FTimerHandle AfterDeadTimerHandle;
	
	//Getter Setter
public:
	__forceinline uint32 GetMonsterHP() { return MonsterCurrentHP; }
	__forceinline void SetMonsterHP(uint32 hp) { MonsterCurrentHP = hp; }

	__forceinline float GetMonsterSpeed() { return MonsterCurrentMoveSpeed; }
	__forceinline void SetMonsterSpeed(float sp) { MonsterCurrentMoveSpeed = sp; }

	__forceinline void SetMonsterState(EState state) { MonsterState = state; }
	__forceinline EState GetMonsterState() { return MonsterState; }

};
