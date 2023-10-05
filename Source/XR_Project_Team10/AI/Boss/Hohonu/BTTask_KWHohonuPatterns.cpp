// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/AI/Boss/Hohonu/BTTask_KWHohonuPatterns.h"

UBTTask_KWHohonuPatterns::UBTTask_KWHohonuPatterns()
{
	
}

void UBTTask_KWHohonuPatterns::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

EBTNodeResult::Type UBTTask_KWHohonuPatterns::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	// 컨트롤 폰 가져와서 캐스팅 한 뒤 애님 몽타주 어느 부분 재생할지 선택해서 실행
	// 
	switch (TaskPattern)
	{
	case EHohonuPattern::SummonCrystal:
		
		break;
	case EHohonuPattern::SweepLaser:
		
		break;
	case EHohonuPattern::MeleeAttack:
		
		break;
	case EHohonuPattern::WhirlWind:
		
		break;
	case EHohonuPattern::BackStep:
		
		break;
	case EHohonuPattern::MultipleLaser:
		
		break;
	default:
		checkNoEntry();
	}
	return EBTNodeResult::InProgress;
}
