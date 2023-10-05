// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/AI/BTTask_KWMoveToNearby.h"
#include "XR_Project_Team10/Constant/KWBlackBoardKeyName.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_KWMoveToNearby::UBTTask_KWMoveToNearby()
{
	
}

void UBTTask_KWMoveToNearby::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if(Blackboard->GetValueAsFloat(KEY_TARGET_DISTANCE) <= NearbyDistance)
	{
		Blackboard->SetValueAsBool(KEY_NEARBY_BOOLEAN, true);
	}
}
