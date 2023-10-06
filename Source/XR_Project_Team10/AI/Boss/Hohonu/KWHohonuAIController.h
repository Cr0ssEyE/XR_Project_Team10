// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "XR_Project_Team10/Character/Monster/Boss/KWBossHohonuAIDataAsset.h"
#include "XR_Project_Team10/Character/Monster/Boss/KWBossMonsterHohonu.h"
#include "KWHohonuAIController.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API AKWHohonuAIController : public AAIController
{
	GENERATED_BODY()

public:
	AKWHohonuAIController();

	void ActivateAI();
	void DeActivateAI();
	void ResetCoolDown(EHohonuPattern Pattern);
	
	UFUNCTION()
	void SetTarget(const TArray<AActor*>& Actors);

	FORCEINLINE UKWBossHohonuAIDataAsset* GetAIDataAsset() { return  AIDataAsset; }
	
protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	void CheckCoolDown(FName KeyName);
	
	// AI 및 컨트롤 액터 관련 변수
private:
	UPROPERTY()
    TObjectPtr<UBlackboardData> BlackboardData;
    
    UPROPERTY()
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AKWBossMonsterHohonu> ControllingPawn;

	UPROPERTY()
	TObjectPtr<UKWBossHohonuAIDataAsset> AIDataAsset;
};
