// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_KWPlayerDetect.h"
#include "XR_Project_Team10/Character/Monster/Boss/KWBossMonsterHohonu.h"
#include "XR_Project_Team10/Constant/KWBlackBoardKeyName.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "XR_Project_Team10/Constant/KWCollisionChannel.h"

UBTService_KWPlayerDetect::UBTService_KWPlayerDetect()
{
	NodeName = TEXT("Detect Target");
	Interval = 1.0f;
}

void UBTService_KWPlayerDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AKWBossMonsterHohonu* ControllingPawn = Cast<AKWBossMonsterHohonu>(OwnerComp.GetAIOwner()->GetPawn());
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

	// TODO: 매직넘버 수정
	float DetectRadius = 5000.f;
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

	if(bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(OverlapResult.GetActor());
			if(PlayerCharacter)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(KEY_TARGET, PlayerCharacter);
				ControllingPawn->SetTarget(*PlayerCharacter);
				DrawDebugSphere(World, Center, DetectRadius, 32, FColor::Green, false, 1.f);
			}
		}
	}
}
