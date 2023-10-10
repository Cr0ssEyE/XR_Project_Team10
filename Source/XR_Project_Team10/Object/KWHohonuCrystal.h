// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Pawn.h"
#include "XR_Project_Team10/Character/Monster/Boss/KWBossHohonuDataAsset.h"
#include "KWHohonuCrystal.generated.h"

UCLASS()
class XR_PROJECT_TEAM10_API AKWHohonuCrystal : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AKWHohonuCrystal();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void ActivateAndDropDownSequence();
	
private:
	void DeActivateSequence();
	void DropDownDelay();
	void DropDownExecute();
	void ActivateWaveAttack();
	void WaveAttackHitCheck();
	void WaveDebug();
	void ActivateWaveAttackTimer();
	void SetDeActivate();
	
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCapsuleComponent> CollisionCapsule;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraComponent> SummonVFX;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraComponent> DropDownVFX;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraComponent> WaveVFX;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraComponent> DestroyVFX;

	UPROPERTY()
	TObjectPtr<UKWBossHohonuDataAsset> BossHohonuDataAsset;
	
private:
	float SC_Hp;
	
	float CurrentHp;

	FTimerHandle DropDownDelayTimerHandle;
	
	FTimerHandle DropDownTimerHandle;

	FTimerHandle WaveAttackHitCheckTimerHandle;

	FTimerHandle WaveAttackDebugTimerHandle;
	
	FTimerHandle WaveAttackDelayTimerHandle;

	FTimerHandle DestroyEventTimerHandle;

	FVector ReBoundVector;
	
	float SC_DropDownDamage;
	
	float SC_DropDownSpeed;

	float SC_DropDownDelay;
	
	float SC_WaveLength;

	float SC_WaveDamage;

	float SC_CurrentAttackDelay;
	
	float SC_AttackDelay;

	float SC_CurrentAttackRange;
	
	float SC_MaxAttackRange;

	float SC_IncreaseAttackRange;

	uint8 bIsDebugEnable : 1;
	
	uint8 bIsActivate : 1;
	
	uint8 bIsAttackOnGoing : 1;

	uint8 bIsPlaceInGround : 1;

	uint8 bIsDropDownDamageCaused : 1;
	
	uint8 bIsWaveDamageCaused : 1;

	uint8 bIsSpawnDelayOnGoing : 1;
};
