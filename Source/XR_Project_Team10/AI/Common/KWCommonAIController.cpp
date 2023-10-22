// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/AI/Common/KWCommonAIController.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"
#include "XR_Project_Team10/Constant/KWBlackBoardKeyName.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "XR_Project_Team10/CommonMonster/CommonMonster.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
#include "XR_Project_Team10/CommonMonster/ICommonMonsterBase.h"

AKWCommonAIController::AKWCommonAIController()
{
	CommonBlackboardData = FPPConstructorHelper::FindAndGetObject<UBlackboardData>(TEXT("/Script/AIModule.BlackboardData'/Game/9-CommonAI/AI/BB_CommonMonster.BB_CommonMonster'"), EAssertionLevel::Check);
	CommonBehaviorTree = FPPConstructorHelper::FindAndGetObject<UBehaviorTree>(TEXT("/Script/AIModule.BehaviorTree'/Game/9-CommonAI/AI/BT_CommonMonster.BT_CommonMonster'"), EAssertionLevel::Check);
	
	CommonPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPercptionComponent"));
	CommonSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI_Sight"));

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
	ACommonMonster* ControllingPawn = Cast<ACommonMonster>(BehaviorTreeComponent->GetAIOwner()->GetPawn());
	for(AActor* DetectActor : Actors)
	{
		FActorPerceptionBlueprintInfo PerceptionInfo;
		PerceptionComponent->GetActorsPerception(DetectActor, PerceptionInfo);
		AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(DetectActor);
		if(PlayerCharacter)
		{
			Blackboard->SetValueAsObject(KEY_TARGET, PlayerCharacter->GetTruePlayerTarget());
			CommonSight->PeripheralVisionAngleDegrees = 180.f;
			return;
		}
	}
}

void AKWCommonAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	IICommonMonsterBase* CommonMonsterBase = Cast<IICommonMonsterBase>(InPawn);
	if (CommonMonsterBase) {
		UBlackboardComponent* BlackboardComponent = Blackboard.Get();
		if (UseBlackboard(CommonBlackboardData, BlackboardComponent))
		{
			Blackboard->SetValueAsObject(KEY_DATAASSET ,CommonMonsterBase->MonsterData);
		}
	}
	CommonSight->SightRadius = CommonMonsterBase->MonsterData->MonsterRecognitionRange;
	CommonSight->LoseSightRadius = CommonMonsterBase->MonsterData->MonsterTrackingRange;
	CommonSight->PeripheralVisionAngleDegrees = 180.f;

	CommonSight->DetectionByAffiliation.bDetectEnemies = true;
	CommonSight->DetectionByAffiliation.bDetectFriendlies = true;
	CommonSight->DetectionByAffiliation.bDetectNeutrals = true;
	CommonSight->SetMaxAge(0.1f);

	CommonPerceptionComp->ConfigureSense(*CommonSight);
	CommonPerceptionComp->SetDominantSense(CommonSight->GetSenseImplementation());
	CommonPerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AKWCommonAIController::SetTarget);

	ActivateAI();
}
