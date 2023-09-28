// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/AI/Common/BTTask_KWCommonFindLocation.h"
#include "XR_Project_Team10/Constant/KWBlackBoardKeyName.h"
#include "XR_Project_Team10/Interface/KWMonsterAIInterface.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_KWCommonFindLocation::UBTTask_KWCommonFindLocation()
{
	
}

EBTNodeResult::Type UBTTask_KWCommonFindLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(!ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if(!NavSystem)
	{
		return EBTNodeResult::Failed;
	}
	
	IKWMonsterAIInterface* MonsterAIInterface = Cast<IKWMonsterAIInterface>(ControllingPawn);
	if(!MonsterAIInterface)
	{
		return EBTNodeResult::Failed;
	}
	
	const FVector OriginLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(KEY_BASE_LOCATION);
	float PatrolRadius = MonsterAIInterface->GetAIPatrolRadius();
	FNavLocation NextPatrolLocation;
	
	if(NavSystem->GetRandomPointInNavigableRadius(OriginLocation, PatrolRadius, NextPatrolLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(KEY_PATROL_LOCATION, NextPatrolLocation.Location);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
