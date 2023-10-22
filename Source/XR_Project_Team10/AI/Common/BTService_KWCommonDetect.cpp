// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/AI/Common/BTService_KWCommonDetect.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "XR_Project_Team10/CommonMonster/CommonMonster.h"
#include "XR_Project_Team10/Constant/KWBlackBoardKeyName.h"
#include "XR_Project_Team10/CommonMonster/ICommonMonsterBase.h"
#include "XR_Project_Team10/Constant/KWCollisionChannel.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"

UBTService_KWCommonDetect::UBTService_KWCommonDetect()
{
	NodeName = TEXT("Detect Target");
	Interval = 1.0f;
}

void UBTService_KWCommonDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACommonMonster* ControllingPawn = Cast<ACommonMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if(!ControllingPawn)
	{
		return;
	}
	FVector Center = ControllingPawn->GetActorLocation();
	
	UWorld* World = ControllingPawn->GetWorld();
	if(!World)
	{
		return;
	}
	
	IICommonMonsterBase* MonsterBase = Cast<IICommonMonsterBase>(ControllingPawn);
	if(!MonsterBase)
	{
		return;
	}

	float DetectRadius = MonsterBase->MonsterData->MonsterRecognitionRange;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(Detect), false, ControllingPawn);

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECC_PLAYER_ONLY,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParams
		);
	bool bIsPlayerDetect = false;
	if(bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(OverlapResult.GetActor());
			if(PlayerCharacter)
			{
				bIsPlayerDetect = true;
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(KEY_TARGET, PlayerCharacter->GetTruePlayerTarget());
				DrawDebugSphere(World, Center, DetectRadius, 32, FColor::Green, false, 0.1f);
				// DrawDebugPoint(World, PlayerCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 1.0f);

				// DrawDebugLine(World, ControllingPawn->GetActorLocation(), PlayerCharacter->GetActorLocation(), FColor::Red, false, 1.0f);
			}
		}
	}
	if(!bIsPlayerDetect)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(KEY_TARGET, nullptr);
	}
}
