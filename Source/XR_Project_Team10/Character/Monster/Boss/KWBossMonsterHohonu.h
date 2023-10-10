// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "XR_Project_Team10/Character/Monster/Boss/KWBossMonsterBase.h"
#include "XR_Project_Team10/Enumeration/KWHohonuPattern.h"
#include "XR_Project_Team10/Object/KWHohonuCrystal.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
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

	FAICharacterPatternFinished CharacterPatternFinished;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void InitData() override;

public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SetAIPatternDelegate(const FAICharacterPatternFinished& PatternFinishedDelegate) override;
	virtual void EndEncounterAnimation() override;
	virtual void PlayPatternAnimMontage() override;
	virtual void PlayDeadAnimation() override;
	virtual void DeActivateInGame() override;
	virtual void ReActivateInGame() override;

	// BT 및 AI 컨트롤러 호출용 함수
public:
	FORCEINLINE float GetHp() const { return BossHp; }
	FORCEINLINE void SetTarget(AKWPlayerCharacter& Actor) { TargetPlayer = Actor; }
	FORCEINLINE void SetPattern(const EHohonuPattern Pattern) { CurrentPattern = Pattern; }
	FORCEINLINE void StopPattern() { bIsPatternRunning = false; }
	
	UFUNCTION()
	void ActivatePatternOmen(UAnimMontage* Montage);
	
	UFUNCTION()
	void ActivatePatternExecute(const EHohonuPattern Pattern);
	
	UFUNCTION()
	void FinishAIPatternNode(UAnimMontage* Montage, bool IsInterrupted);
	
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
	// 각 패턴 경고 / 전조 효과
	void OmenPattern_SC();
	void OmenPattern_SL();
	void OmenPattern_MA();
	void OmenPattern_WW();
	void OmenPattern_BS();
	void OmenPattern_ML();

	// 각 패턴 구현부
    void ExecutePattern_SC();
    void ExecutePattern_SL();
    void ExecutePattern_MA();
    void ExecutePattern_WW();
    void ExecutePattern_BS();
    void ExecutePattern_ML();

	// 호호누 패턴 관련 변수
private:
	UPROPERTY()
	TObjectPtr<class UKWBossHohonuAnimInstance> HohonuAnimInstance;
	
	UPROPERTY()
	TObjectPtr<class AKWPlayerCharacter> TargetPlayer;
	
	EHohonuPattern CurrentPattern;
	
	float HohonuLunaticHp;

	FVector ReBoundVector;
	
	// 수정 소환 관련 변수
private:
	UPROPERTY(EditAnywhere, DisplayName = "수정 인스턴스")
	TArray<AKWHohonuCrystal*> SC_Instances;
	
	FTimerHandle SC_SpawnTimerHandle;

	int SC_SpawnCount;
	
	float SC_SpawnDelay;

	float SC_SpawnHeight;

	uint8 bIsCanSummon : 1;
	
	// 레이저 발사 관련 변수
private:
	FTimerHandle SL_SweepTimerHandle;

	uint8 SL_bIsRandomStart : 1;
	
	float SL_Damage;

	float SL_Degree;

	float SL_ActiveTime;

	float SL_Distance;
	
	FVector SL_DamageRange;

	uint8 bIsSweepLaserDamageCaused : 1;

	uint8 bIsSweepLeftToRight : 1;
	
	// 근접 공격 관련 변수
private:
	FTimerHandle MA_TimerHandle;
	
	float MA_Damage;

	FVector MA_DamageRange;

	FVector MA_ExplodeDamageRange;
	
	uint8 bIsMeleeAttackDamageCaused : 1;
	
	// 훨윈드 관련 변수
private:
	FTimerHandle WW_TimerHandle;
	
	float WW_Damage;

	FVector WW_DamageRange;
	
	float WW_AttackDelay;
	
	float WW_AttackTime;
	
	float WW_IncreaseMoveSpeedPerSecond;

	float WW_MaxMoveSpeed;

	float WW_RotateSpeed;

	uint8 bIsWhirlWindDamageCaused : 1;
	
	// 백스텝 관련 변수
private:
	FTimerHandle BackStepTimerHandle;
	
	float BS_Range;

	float BS_MoveSpeed;

	float BS_ElapsedTime;
	
	// 레이저 난사 관련 변수
	// 중간 시연 이후에 작업
private:
	float ML_Damage;

	float ML_AttackTime;

	float ML_TurnSpeed;

	// 기타 효과 관련 컴포넌트
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraComponent> HohonuRingEffect;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraComponent> HohonuHeadEffect;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraComponent> HohonuLaserChargeEffect;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraComponent> HohonuLaserSweepEffect;

	UPROPERTY()
	uint8 bIsDebugEnable : 1;
};
