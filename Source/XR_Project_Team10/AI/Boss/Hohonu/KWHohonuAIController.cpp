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
	BlackboardData = FPPConstructorHelper::FindAndGetObject<UBlackboardData>(TEXT("/Script/AIModule.BlackboardData'/Game/21-Hohonu/AI/Hohonu/BB_BossHohonu.BB_BossHohonu'"));
	BehaviorTree = FPPConstructorHelper::FindAndGetObject<UBehaviorTree>(TEXT("/Script/AIModule.BehaviorTree'/Game/21-Hohonu/AI/Hohonu/BT_BossHohonu.BT_BossHohonu'"));
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

void AKWHohonuAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(Blackboard->GetValueAsObject(KEY_TARGET) == nullptr)
	{
		TArray<FHitResult> HitResults;
		FCollisionQueryParams Params(NAME_None, false, this);
			
		bool bResult = GetWorld()->SweepMultiByChannel(
		HitResults,
		GetPawn()->GetActorLocation(),
		GetPawn()->GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeBox(FVector(1000.f, 1000.f, 200.f)),
		Params);

		TArray<AActor*> Actors;
		for(auto Result : HitResults)
		{
			if(AActor* ResultActor = Result.GetActor())
			{
				Actors.Emplace(ResultActor);
			}
		}
		SetTarget(Actors);
	}
}

void AKWHohonuAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ActivateAI();
}
