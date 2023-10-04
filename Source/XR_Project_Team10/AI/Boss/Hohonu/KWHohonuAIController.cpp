// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/AI/Boss/Hohonu/KWHohonuAIController.h"
#include "XR_Project_Team10/Constant/KWBlackBoardKeyName.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "XR_Project_Team10/Character/Monster/Boss/KWBossMonsterHohonu.h"

AKWHohonuAIController::AKWHohonuAIController()
{
	
}

void AKWHohonuAIController::ActivateAI()
{
	UBlackboardComponent* BlackboardComponent = Blackboard.Get();
	if(UseBlackboard(BlackboardData, BlackboardComponent))
	{
		Blackboard->SetValueAsVector(KEY_BASE_LOCATION, GetPawn()->GetActorLocation());
		
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
		FActorPerceptionBlueprintInfo PerceptionInfo;
		PerceptionComponent->GetActorsPerception(DetectActor, PerceptionInfo);
		AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(DetectActor);
		if(PlayerCharacter)
		{
			Blackboard->SetValueAsObject(KEY_TARGET, PlayerCharacter);
			AKWBossMonsterHohonu* ControllingPawn = Cast<AKWBossMonsterHohonu>(GetPawn());
			if(ControllingPawn)
			{
				ControllingPawn->SetTarget(*PlayerCharacter);
			}
			return;
		}
	}
}

void AKWHohonuAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ActivateAI();
}
