// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XR_Project_Team10/CommonMonster/CommonMonster.h"
#include "RuneSpirit.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API ARuneSpirit : public ACommonMonster
{
	GENERATED_BODY()
public:
	ARuneSpirit();

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void AttackOmen() override;
	virtual void Attack() override;
	virtual void AttackEnd() override;

	virtual void AttackBehaviour() override;

	virtual void PlayDeadAnimation() override;
	virtual void PlayHitAnimation() override;

private:
	UPROPERTY()
	FVector ReBoundVector;
};
