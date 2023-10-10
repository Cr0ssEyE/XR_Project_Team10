// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/AI/Common/BTTask_CommonAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "XR_Project_Team10/Constant/KWBlackBoardKeyName.h"
#include "XR_Project_Team10/CommonMonster/ICommonMonsterBase.h"

UBTTask_CommonAttack::UBTTask_CommonAttack()
{
}

EBTNodeResult::Type UBTTask_CommonAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn) {
		return EBTNodeResult::Failed;
	}

	IICommonMonsterBase* AIPawn = Cast<IICommonMonsterBase>(ControllingPawn);
	if (nullptr == AIPawn) {
		return EBTNodeResult::Failed;
	}

	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(KEY_TARGET));
	if (nullptr == TargetActor) {
		return EBTNodeResult::Failed;
	}


	FCommonAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]() {
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);
	AIPawn->SetCommonAttackDelegate(OnAttackFinished);
	AIPawn->CommonMonsterAttack(TargetActor);
	return EBTNodeResult::InProgress;
}
