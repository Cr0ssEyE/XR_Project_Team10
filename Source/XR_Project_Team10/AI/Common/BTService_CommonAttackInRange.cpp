// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/AI/Common/BTService_CommonAttackInRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "XR_Project_Team10/Constant/KWBlackBoardKeyName.h"
#include "XR_Project_Team10/CommonMonster/ICommonMonsterBase.h"
#include "XR_Project_Team10/Interface/KWMonsterAIInterface.h"
#include "XR_Project_Team10/Object/KWLocationDetector.h"

UBTService_CommonAttackInRange::UBTService_CommonAttackInRange()
{
	NodeName = TEXT("CanAttack");
}

void UBTService_CommonAttackInRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	IICommonMonsterBase* AIPawn = Cast<IICommonMonsterBase>(ControllingPawn);
	if (nullptr == AIPawn)
		{
		return;
	}

	AKWLocationDetector* Target = Cast<AKWLocationDetector>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(KEY_TARGET));
	if (nullptr == Target)
		{
		return;
	}

	float DistanceToTarget = FVector::DistXY(ControllingPawn->GetActorLocation(), Target->GetActorLocation());
	float AttackRangeWithRadius = AIPawn->MonsterData->MonsterAttackConfigRange;
	if(DistanceToTarget <= AttackRangeWithRadius)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(KEY_NEARBY_BOOLEAN, true);
		return;
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(KEY_NEARBY_BOOLEAN, false);
}
