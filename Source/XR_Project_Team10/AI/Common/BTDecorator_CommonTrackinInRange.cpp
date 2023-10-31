// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/AI/Common/BTDecorator_CommonTrackinInRange.h"
#include "BTDecorator_CommonTrackinInRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "XR_Project_Team10/Constant/KWBlackBoardKeyName.h"
#include "XR_Project_Team10/CommonMonster/ICommonMonsterBase.h"
#include "XR_Project_Team10/Interface/KWMonsterAIInterface.h"
#include "XR_Project_Team10/Object/KWLocationDetector.h"

UBTDecorator_CommonTrackinInRange::UBTDecorator_CommonTrackinInRange()
{
	NodeName = TEXT("CanTracking");
}

bool UBTDecorator_CommonTrackinInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	IICommonMonsterBase* AIPawn = Cast<IICommonMonsterBase>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return false;
	}

	AKWLocationDetector* Target = Cast<AKWLocationDetector>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(KEY_TARGET));
	if (nullptr == Target)
	{
		return false;
	}

	//UE_LOG(LogTemp, Log, TEXT("%d %d %d"), Target->GetOwner()->GetActorLocation().X, Target->GetOwner()->GetActorLocation().Y, Target->GetOwner()->GetActorLocation().Z);
	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	float TrackingRangeWithRadius = AIPawn->MonsterData->MonsterTrackingRange;
	bResult = DistanceToTarget <= TrackingRangeWithRadius;
	//UE_LOG(LogTemp, Log, TEXT("Distance : %f, Tracking : %f, Bool : %d"),DistanceToTarget,TrackingRangeWithRadius,bResult ? 1 : 0);
	return bResult;
}
