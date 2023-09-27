// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/AI/Common/KWCommonAIController.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"
#include "XR_Project_Team10/Constant/KWBlackBoardKeyName.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "XR_Project_Team10/Character/Monster/Common/KWDummyMonster.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"

AKWCommonAIController::AKWCommonAIController()
{
	CommonBlackboardData = FPPConstructorHelper::FindAndGetObject<UBlackboardData>(TEXT("/Script/AIModule.BlackboardData'/Game/9-CommonAI/AI/BB_CommonMonster.BB_CommonMonster'"), EAssertionLevel::Check);
	CommonBehaviorTree = FPPConstructorHelper::FindAndGetObject<UBehaviorTree>(TEXT("/Script/AIModule.BehaviorTree'/Game/9-CommonAI/AI/BT_CommonMonster.BT_CommonMonster'"), EAssertionLevel::Check);
	
	CommonPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPercptionComponent"));
	CommonSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI_Sight"));

	// 테스트용 매직 넘버 사용.
	// 나중에 세부 구현 부분에서는 데이터 에셋으로 가져와서 사용하기
	CommonSight->SightRadius = 500.f;
	CommonSight->LoseSightRadius = 800.f;
	CommonSight->PeripheralVisionAngleDegrees = 60.f;
	
	CommonSight->DetectionByAffiliation.bDetectEnemies = true;
	CommonSight->DetectionByAffiliation.bDetectFriendlies = true;
	CommonSight->DetectionByAffiliation.bDetectNeutrals = true;
	CommonSight->SetMaxAge(0.1f);
	
	CommonPerceptionComp->ConfigureSense(*CommonSight);
	CommonPerceptionComp->SetDominantSense(CommonSight->GetSenseImplementation());
	CommonPerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AKWCommonAIController::SetTarget);
}

void AKWCommonAIController::ActivateAI()
{
	UBlackboardComponent* BlackboardComponent = Blackboard.Get();
	if(UseBlackboard(CommonBlackboardData, BlackboardComponent))
	{
		Blackboard->SetValueAsVector(KEY_BASE_LOCATION, GetPawn()->GetActorLocation());
		
		bool RunResult = RunBehaviorTree(CommonBehaviorTree);
		ensure(RunResult);
	}
}

void AKWCommonAIController::DeActivateAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if(BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree();
	}
}

void AKWCommonAIController::SetTarget(const TArray<AActor*>& Actors)
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	AKWDummyMonster* ControllingPawn = Cast<AKWDummyMonster>(BehaviorTreeComponent->GetAIOwner()->GetPawn());
	for(AActor* DetectActor : Actors)
	{
		FActorPerceptionBlueprintInfo PerceptionInfo;
		PerceptionComponent->GetActorsPerception(DetectActor, PerceptionInfo);
		AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(DetectActor);
		if(PlayerCharacter)
		{
			Blackboard->SetValueAsObject(KEY_TARGET, PlayerCharacter);
			CommonSight->PeripheralVisionAngleDegrees = 60.f;
			return;
		}
	}
}

void AKWCommonAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ActivateAI();
}
