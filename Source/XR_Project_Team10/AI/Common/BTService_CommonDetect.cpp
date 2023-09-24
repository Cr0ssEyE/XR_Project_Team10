// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/AI/Common/BTService_CommonDetect.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "XR_Project_Team10/Constant/KWBlackBoardKeyName.h"
#include "XR_Project_Team10/Interface/KWMonsterAIInterface.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"

UBTService_CommonDetect::UBTService_CommonDetect()
{
	NodeName = TEXT("Detect Target");
	Interval = 1.0f;
}

void UBTService_CommonDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
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
	
	IKWMonsterAIInterface* MonsterAIInterface = Cast<IKWMonsterAIInterface>(ControllingPawn);
	if(!MonsterAIInterface)
	{
		return;
	}

	float DetectRadius = MonsterAIInterface->GetAIDetectRange();
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(Detect), false, ControllingPawn);

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParams
		);

	if(bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(OverlapResult.GetActor());
			if(PlayerCharacter)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(KEY_TARGET, PlayerCharacter);
				DrawDebugSphere(World, Center, DetectRadius, 32, FColor::Green, false, 1.f);

				DrawDebugPoint(World, PlayerCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 1.0f);

				DrawDebugLine(World, ControllingPawn->GetActorLocation(), PlayerCharacter->GetActorLocation(), FColor::Red, false, 1.0f);
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(KEY_TARGET, nullptr);
		DrawDebugSphere(World, Center, DetectRadius, 32, FColor::Yellow, false, 1.f);
	}
}
