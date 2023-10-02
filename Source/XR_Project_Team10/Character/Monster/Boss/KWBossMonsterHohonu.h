// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "XR_Project_Team10/Character/Monster/Boss/KWBossMonsterBase.h"
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
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void PlayEncounterAnimation() override;
	virtual void PlayDeadAnimation() override;
	virtual void DeActivateInGame() override;
	virtual void ReActivateInGame() override;

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
public:
    void ExecutePattern_SC(UBehaviorTreeComponent& OwnerComp);
    void ExecutePattern_SL(UBehaviorTreeComponent& OwnerComp);
    void ExecutePattern_MA(UBehaviorTreeComponent& OwnerComp);
    void ExecutePattern_WW(UBehaviorTreeComponent& OwnerComp);
    void ExecutePattern_BS(UBehaviorTreeComponent& OwnerComp);
    void ExecutePattern_ML(UBehaviorTreeComponent& OwnerComp);
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void InitData() override;
	
};
