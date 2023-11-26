// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/AI/BTDecorator_KWBackDistanceCheck.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "XR_Project_Team10/Constant/KWBlackBoardKeyName.h"
#include "XR_Project_Team10/Constant/KWCollisionChannel.h"
#include "XR_Project_Team10/Object/KWLocationDetector.h"

UBTDecorator_KWBackDistanceCheck::UBTDecorator_KWBackDistanceCheck()
{
	
}

bool UBTDecorator_KWBackDistanceCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	AKWLocationDetector* Target = Cast<AKWLocationDetector>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(KEY_TARGET));
	if (nullptr == Target)
	{
		return false;
	}

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, ControllingPawn);
	bool ObjectResult = GetWorld()->LineTraceSingleByProfile(
		HitResult,
		ControllingPawn->GetActorLocation(),
		ControllingPawn->GetActorLocation() - ControllingPawn->GetActorForwardVector() * BackyardDistance,
		CP_STATIC_ONLY,
		Params
	);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("후방 거리 체크: %d"), ObjectResult));
	if(ObjectResult)
	{
		return false;
	}
	
	return true;
}
