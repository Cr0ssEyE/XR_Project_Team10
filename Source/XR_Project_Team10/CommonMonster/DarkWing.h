// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XR_Project_Team10/CommonMonster/CommonMonster.h"
#include "XR_Project_Team10/CommonMonster/DW_FeatherProjectile.h"
#include "DarkWing.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API ADarkWing : public ACommonMonster
{
	GENERATED_BODY()
public:
	ADarkWing();

	//virtual bool ShouldTickIfViewportsOnly() const override { return true; }
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	// IICommonMonsterBase을(를) 통해 상속
protected:
	virtual void BeginPlay() override;
	
	virtual void AttackOmen(AActor* Target) override;
	virtual void Attack(AActor* Target) override;

	
	// 외부 수정 변수
private:
	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "공격 준비 시간")
	float AttackReadyTime = 0.5;

	UPROPERTY(EditAnywhere, Category = Attack, DisplayName = "공격 사거리")
	float AttackRange;
	
	UPROPERTY(EditAnywhere, Category = FeatherAttack, DisplayName = "깃털 공격력")
	uint32 FeatherPower;

	UPROPERTY(EditAnywhere, Category = FeatherAttack, DisplayName = "깃털 속도")
	float FeatherSpeed;

	UPROPERTY(EditAnywhere, Category = FeatherAttack, DisplayName = "날리는 깃털 갯수")
	uint32 FeatherNum;

	UPROPERTY(EditAnywhere, Category = FeatherAttack, DisplayName = "깃털 삭제되는 시간")
	float FeatherDeleteTime;

	// 내부 수정 변수
private:
	UPROPERTY(EditDefaultsOnly, Category = FeatherAttack)
	TSubclassOf<class ADW_FeatherProjectile> FeatherClass;

	const FName FeatherSockets[2] = { TEXT("Feather_lSocket"), TEXT("Feather_rSocket") };
};
