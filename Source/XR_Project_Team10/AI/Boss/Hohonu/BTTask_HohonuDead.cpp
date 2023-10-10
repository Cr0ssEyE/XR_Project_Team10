// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/AI/Boss/Hohonu/BTTask_HohonuDead.h"

#include "KWHohonuAIController.h"

UBTTask_HohonuDead::UBTTask_HohonuDead()
{
	
}

EBTNodeResult::Type UBTTask_HohonuDead::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AKWHohonuAIController* AIController = Cast<AKWHohonuAIController>(OwnerComp.GetAIOwner());
	if(AIController)
	{
		AIController->DeActivateAI();
	}
	return EBTNodeResult::Succeeded;
}

