// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "XR_Project_Team10/Character/Monster/Boss/KWBossMonsterBase.h"
#include "XR_Project_Team10/Enumeration/KWHohonuPattern.h"
#include "KWBossMonsterHohonu.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API AKWBossMonsterHohonu : public AKWBossMonsterBase
{
	GENERATED_BODY()

public:
	AKWBossMonsterHohonu();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void InitData() override;

public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void PlayEncounterAnimation() override;
	virtual void PlayDeadAnimation() override;
	virtual void DeActivateInGame() override;
	virtual void ReActivateInGame() override;

public:
	void ActivatePatternOmen(EHohonuPattern Pattern);
	void TogglePattern(EHohonuPattern Pattern, bool Value);
	/**
	* 호호누 패턴 구현 함수 \n
	* 호호누 패턴 별 약자 \n
	* 수정 소환 = Summon Crystal = SC_ \n
	* 레이저 발사 = SweepLaser = SL_ \n
	* 양손 휘두르기 = MeleeAttack = MA_ \n
	* 훨윈드 = Whirlwind = WW_ \n
	* 백스텝 = BackStep = BS_ \n
	* 연발 레이저 = MultipleLaser = ML_
	**/
private:
	void OmenPattern_SC();
	void OmenPattern_SL();
	void OmenPattern_MA();
	void OmenPattern_WW();
	void OmenPattern_BS();
	void OmenPattern_ML();
    void ExecutePattern_SC(bool Value);
    void ExecutePattern_SL(bool Value);
    void ExecutePattern_MA(bool Value);
    void ExecutePattern_WW(bool Value);
    void ExecutePattern_BS(bool Value);
    void ExecutePattern_ML(bool Value);

	// 호호누 패턴 관련 변수
private:
	float HohonuLunaticHp;
	
	float SC_Count;
	
	float SC_Speed;

	float SL_Damage;

	float SL_AttackDelay;
	
	float SL_Degree;

	float SL_TurnSpeed;
	
	float SL_AttackRange;

	float MA_Damage;
	
	float MA_AttackSpeed;
	
	float WW_Damage;

	float WW_AttackDelay;
	
	float WW_AttackTime;
	
	float WW_IncreaseMoveSpeed;

	float WW_MaxMoveSpeed;
	
	float BS_Range;

	float BS_MoveSpeed;

	float ML_Damage;

	float ML_AttackTime;

	float ML_TurnSpeed;

	// 기타 효과 관련 컴포넌트
private:
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> HohonuRingEffect;
	
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> HohonuHeadEffect;
	
};
