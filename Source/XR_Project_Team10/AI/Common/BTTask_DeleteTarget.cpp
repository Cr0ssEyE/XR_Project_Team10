// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/AI/Common/BTTask_DeleteTarget.h"
#include "BTTask_DeleteTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "XR_Project_Team10/Constant/KWBlackBoardKeyName.h"
#include "XR_Project_Team10/CommonMonster/ICommonMonsterBase.h"

UBTTask_DeleteTarget::UBTTask_DeleteTarget()
{
}

EBTNodeResult::Type UBTTask_DeleteTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(KEY_TARGET, nullptr);
	return EBTNodeResult::Succeeded;
}
