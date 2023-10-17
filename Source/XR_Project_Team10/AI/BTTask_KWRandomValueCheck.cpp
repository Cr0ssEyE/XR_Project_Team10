// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/AI/BTTask_KWRandomValueCheck.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_KWRandomValueCheck::UBTTask_KWRandomValueCheck()
{
	
}

EBTNodeResult::Type UBTTask_KWRandomValueCheck::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	Blackboard->SetValueAsInt(BlackboardKey.SelectedKeyName, FMath::RandRange(0, RandMax));
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
