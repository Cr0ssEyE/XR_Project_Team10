// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/AI/Boss/Hohonu/KWHohonuAIController.h"
#include "XR_Project_Team10/Constant/KWBlackBoardKeyName.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "XR_Project_Team10/Character/Monster/Boss/KWBossMonsterHohonu.h"
#include "XR_Project_Team10/Util/PPTimerHelper.h"

AKWHohonuAIController::AKWHohonuAIController()
{
	BlackboardData = FPPConstructorHelper::FindAndGetObject<UBlackboardData>(TEXT("/Script/AIModule.BlackboardData'/Game/21-Hohonu/AI/Hohonu/BB_BossHohonu.BB_BossHohonu'"));
	BehaviorTree = FPPConstructorHelper::FindAndGetObject<UBehaviorTree>(TEXT("/Script/AIModule.BehaviorTree'/Game/21-Hohonu/AI/Hohonu/BT_BossHohonu.BT_BossHohonu'"));
}

void AKWHohonuAIController::ActivateAI()
{
	UBlackboardComponent* BlackboardComponent = Blackboard.Get();
	ControllingPawn = Cast<AKWBossMonsterHohonu>(GetPawn());
	if(UseBlackboard(BlackboardData, BlackboardComponent) && ControllingPawn)
	{
		Blackboard->SetValueAsVector(KEY_BASE_LOCATION, GetPawn()->GetActorLocation());
		Blackboard->SetValueAsFloat(KEY_MONSTER_HP, ControllingPawn->GetHp());
		Blackboard->SetValueAsFloat(KEY_HOHONU_SC_COOLDOWN, 0.f);
		Blackboard->SetValueAsFloat(KEY_HOHONU_SL_COOLDOWN, 0.f);
		Blackboard->SetValueAsFloat(KEY_HOHONU_MA_COOLDOWN, 0.f);
		Blackboard->SetValueAsFloat(KEY_HOHONU_WW_COOLDOWN, 0.f);
		Blackboard->SetValueAsFloat(KEY_HOHONU_BS_COOLDOWN, 0.f);
		Blackboard->SetValueAsFloat(KEY_HOHONU_ML_COOLDOWN, 0.f);

		AIDataAsset = CastChecked<UKWBossHohonuAIDataAsset>(ControllingPawn->GetBossAIData());
		bool RunResult = RunBehaviorTree(BehaviorTree);
		ensure(RunResult);
	}
}

void AKWHohonuAIController::DeActivateAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if(BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree();
	}
}

void AKWHohonuAIController::SetTarget(const TArray<AActor*>& Actors)
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	for(AActor* DetectActor : Actors)
	{
		AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(DetectActor);
		if(PlayerCharacter)
		{
			Blackboard->SetValueAsObject(KEY_TARGET, PlayerCharacter);
			ControllingPawn = Cast<AKWBossMonsterHohonu>(GetPawn());
			if(ControllingPawn)
			{
				ControllingPawn->SetTarget(*PlayerCharacter);
			}
			return;
		}
	}
}

void AKWHohonuAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Blackboard->SetValueAsFloat(KEY_MONSTER_HP, ControllingPawn->GetHp());
	CheckCoolDown(KEY_HOHONU_SC_COOLDOWN);
	CheckCoolDown(KEY_HOHONU_SL_COOLDOWN);
	CheckCoolDown(KEY_HOHONU_MA_COOLDOWN);
	CheckCoolDown(KEY_HOHONU_WW_COOLDOWN);
	CheckCoolDown(KEY_HOHONU_BS_COOLDOWN);
	CheckCoolDown(KEY_HOHONU_ML_COOLDOWN);
}

void AKWHohonuAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ActivateAI();
}

void AKWHohonuAIController::CheckCoolDown(FName KeyName)
{
	float CurrentCoolDown = Blackboard->GetValueAsFloat(KeyName);
	if(CurrentCoolDown > 0.f)
	{
		CurrentCoolDown -= GetWorld()->DeltaTimeSeconds;
		Blackboard->SetValueAsFloat(KeyName, CurrentCoolDown);
		if(CurrentCoolDown < 0.f)
		{
			Blackboard->SetValueAsFloat(KeyName, 0.f);
		}
	}
}

void AKWHohonuAIController::ResetCoolDown(EHohonuPattern Pattern)
{
	switch (Pattern)
	{
	case EHohonuPattern::SummonCrystal:
		Blackboard->SetValueAsFloat(KEY_HOHONU_SC_COOLDOWN, AIDataAsset->SC_CoolDown);
		break;
		
	case EHohonuPattern::SweepLaser:
		Blackboard->SetValueAsFloat(KEY_HOHONU_SL_COOLDOWN, AIDataAsset->SL_CoolDown);
		break;
		
	case EHohonuPattern::MeleeAttack:
		Blackboard->SetValueAsFloat(KEY_HOHONU_MA_COOLDOWN, AIDataAsset->MA_CoolDown);
		break;
		
	case EHohonuPattern::WhirlWind:
		Blackboard->SetValueAsFloat(KEY_HOHONU_WW_COOLDOWN, AIDataAsset->WW_CoolDown);
		break;
		
	case EHohonuPattern::BackStep:
		Blackboard->SetValueAsFloat(KEY_HOHONU_BS_COOLDOWN, AIDataAsset->BS_CoolDown);
		break;
		
	case EHohonuPattern::MultipleLaser:
		Blackboard->SetValueAsFloat(KEY_HOHONU_ML_COOLDOWN, AIDataAsset->ML_CoolDown);
		break;
		
	default:
		checkNoEntry();
	}
}
