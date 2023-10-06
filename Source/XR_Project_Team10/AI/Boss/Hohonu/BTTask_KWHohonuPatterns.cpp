// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/AI/Boss/Hohonu/BTTask_KWHohonuPatterns.h"

#include "AIController.h"
#include "KWHohonuAIController.h"
#include "XR_Project_Team10/Character/Monster/Boss/KWBossMonsterHohonu.h"

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

	AKWBossMonsterHohonu* ControllingPawn = Cast<AKWBossMonsterHohonu>(OwnerComp.GetAIOwner()->GetPawn());
	if(!ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	FAICharacterPatternFinished PatternFinished;
	PatternFinished.AddLambda([&]()
	{
		CastChecked<AKWHohonuAIController>(OwnerComp.GetAIOwner())->ResetCoolDown(TaskPattern);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	});
	ControllingPawn->SetAIPatternDelegate(PatternFinished);
	ControllingPawn->SetPattern(TaskPattern);
	ControllingPawn->PlayPatternAnimMontage();
	return EBTNodeResult::InProgress;
}
