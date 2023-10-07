// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/AI/Common/BTDecorator_CommonAttackInRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "XR_Project_Team10/Constant/KWBlackBoardKeyName.h"
#include "XR_Project_Team10/CommonMonster/ICommonMonsterBase.h"
#include "XR_Project_Team10/Interface/KWMonsterAIInterface.h"

UBTDecorator_CommonAttackInRange::UBTDecorator_CommonAttackInRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_CommonAttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UE_LOG(LogTemp, Log, TEXT("Attack"));
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) {
		return false;
	}

	IICommonMonsterBase* AIPawn = Cast<IICommonMonsterBase>(ControllingPawn);
	if (nullptr == AIPawn) {
		return false;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(KEY_TARGET));
	if (nullptr == Target) {
		return false;
	}

	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	float AttackRangeWithRadius = AIPawn->MonsterData->MonsterAttackConfigRange;
	bResult = (DistanceToTarget <= AttackRangeWithRadius);
	return bResult;
}
