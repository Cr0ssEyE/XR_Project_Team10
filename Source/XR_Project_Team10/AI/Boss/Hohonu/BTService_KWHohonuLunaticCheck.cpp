// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/AI/Boss/Hohonu/BTService_KWHohonuLunaticCheck.h"
#include "XR_Project_Team10/Character/Monster/Boss/KWBossHohonuDataAsset.h"
#include "XR_Project_Team10/Character/Monster/Boss/KWBossMonsterHohonu.h"
#include "XR_Project_Team10/Constant/KWBlackBoardKeyName.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_KWHohonuLunaticCheck::UBTService_KWHohonuLunaticCheck()
{
	
}

void UBTService_KWHohonuLunaticCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	
	AKWBossMonsterHohonu* ControllingPawn = Cast<AKWBossMonsterHohonu>(OwnerComp.GetAIOwner()->GetPawn());
	ensure(ControllingPawn);
	
	UKWBossHohonuDataAsset* DataAsset = Cast<UKWBossHohonuDataAsset>(ControllingPawn->GetBossStatusData());
	ensure(DataAsset);

	if(Blackboard->GetValueAsFloat(KEY_MONSTER_HP) < DataAsset->HohonuLunaticHp)
	{
		Blackboard->SetValueAsBool(KEY_HOHONU_LUNATIC_BOOLEAN, true);
	}
}
